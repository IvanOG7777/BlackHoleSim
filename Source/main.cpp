#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "../Header/Constants.h"
#include "../Header/Particle.h"
#include "../Header/Renderer.h"
#include "../Header/Physics.h"
#include "../Header/Input.h"

const char *vertexShader = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPos; // Current vertex we are handling from the vector of vertices

        uniform vec2 uResolution; //holds screen W/H
        uniform vec2 uOffset; // current position of the particle
        uniform float uScale; //  particles radius

        void main() {
            vec2 worldPos = aPos.xy * uScale + uOffset; // only use the xy values of the vertices, scale it and add the current position to get real new position

            vec2 ndc = worldPos / uResolution; // convert from pixel coordinated to normalized range [0,1]
            ndc = ndc * 2.0 - 1.0; // convert from normalized range to normalized device coordinates [-1,1]

            gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0); // Final position in clip space (OpenGL expects vec4 in NDC range)
        }
    )GLSL";

const char *fragmentShader = R"GLSL(
        #version 330 core
        out vec4 FragColor; // color that will be sent out to screen
        uniform vec3 uColor; // RGB
        void main() {
            FragColor = vec4(uColor, 1.0); // pass in the RBG plus alpha
        }
    )GLSL";

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit()) {
        std::cerr << "GLFW INIT ERROR \n";
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(W, H, "Hello World", NULL, NULL);
    if (window == nullptr) {
        std::cerr << "WINDOW ERROR, COULDN'T CREATE, WINDOW IS NULL\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "GLAD INIT ERROR\n";
        return -1;
    }

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, nullptr);
    glCompileShader(fs);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint uResolutionLoc = glGetUniformLocation(shaderProgram, "uResolution");
    GLuint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLuint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    GLuint uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");

    SceneState sceneState{};
    Particle defaultParticle;
    Particle BH;
    BH.setPosition(960, 540, 0);
    BH.setRadius(25);
    BH.setMass(100);
    std:: vector<Particle> particles;
    particles.reserve(10);

    for (size_t i = 0; i < 10; i++) {
        Particle particle;
        setParticleOrbit(particle, BH, 1.0f);
        particles.emplace_back(particle);
    }

    for (auto &particle : particles) {
        particle.printPosition();
        particle.printVelocity();
        particle.printAcceleration();
        std:: cout << "Damp: " << particle.getDamping() << std:: endl;
        std:: cout << "Radius: " << particle.getRadius() << std:: endl;
        std:: cout << std:: endl;
    }

    setParticleOrbit(defaultParticle, BH, 1.0f);

    std:: vector<std::vector<Vector3>> vectorParticleTrails(10);
    for (int i = 0; i < 10; i++) {
        vectorParticleTrails[i].emplace_back(particles[i].getPosition());
    }

    std::vector<Vector3> defaultParticleVertices;
    std::vector<Vector3> BHVertices;
    std::vector<Vector3> defaultTrailPositions;

    defaultParticleVertices = makeUnitCircle(defaultParticle.getRadius());
    BHVertices = makeUnitCircle(BH.getRadius());
    defaultTrailPositions.emplace_back(defaultParticle.getPosition());

    sceneState.particle = &defaultParticle;
    sceneState.blackhole = &BH;
    sceneState.trailPositions = &defaultTrailPositions;
    sceneState.particles = &particles;

    glfwSetWindowUserPointer(window, &sceneState);
    glfwSetKeyCallback(window, keyCallBack);


    GLuint VAO = 0, BHVAO = 0, trailVAO = 0;
    GLuint VBO = 0, BHVBO = 0, trailVBO = 0;

    setVAO(VAO, VBO, GL_DYNAMIC_DRAW, defaultParticleVertices);
    setVAO(BHVAO, BHVBO, GL_DYNAMIC_DRAW, BHVertices);
    setVAO(trailVAO, trailVBO, GL_DYNAMIC_DRAW, defaultTrailPositions);

    bool hasCaptured = false;
    float accumulatedTime = 0;
    float diagnosticAccumulatedTime = 0;

    auto startTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - startTime;
        startTime = currentTime;
        auto dt = deltaTime.count();

        accumulatedTime += dt;
        diagnosticAccumulatedTime += dt;

        int w = W;
        int h = H;
        int particleIndex = 0;

        if (accumulatedTime >= FIXED_DT) {
            accumulatedTime -= FIXED_DT;
            dt = FIXED_DT;
            defaultParticle.update(dt);
            for (auto &particle : particles) {
                particle.update(dt);
            }
        }
        Vector3 acceleration = gravitationalAcceleration(BH.getPosition(), defaultParticle.getPosition(), MU);
        defaultParticle.setAcceleration(acceleration);
        recordTrail(defaultTrailPositions, defaultParticle.getPosition());

        for (auto &particle : particles) {
            Vector3 acc = gravitationalAcceleration(BH.getPosition(), particle.getPosition(), MU);
            particle.setAcceleration(acc);
            recordTrail(vectorParticleTrails[particleIndex], particle.getPosition());
        }

        hasCaptured = hasBeenCaptured(BH.getPosition(), defaultParticle.getPosition(), BH.getRadius());
        if (hasCaptured == true) {

        }

        float particleRadius = orbitalRadius(BH.getPosition(), defaultParticle.getPosition());
        float particleSpeed = speed(defaultParticle.getVelocity());
        float particleEnergy = orbitalEnergy(BH.getPosition(), defaultParticle.getPosition(), defaultParticle.getVelocity(), MU);
        float particleMomentum = angularMomentum(BH.getPosition(), defaultParticle.getPosition(), defaultParticle.getVelocity());
        std:: string particleOrbitType = orbitType(particleEnergy);

        if (diagnosticAccumulatedTime >= DIAGNOSTIC_TIME) {
            diagnosticAccumulatedTime -= DIAGNOSTIC_TIME;
            std:: cout << "Radius: " << particleRadius << "\n";
            std:: cout << "Speed: " << particleSpeed << "\n";
            std:: cout << "Energy: " << particleEnergy << "\n";
            std:: cout << "Momentum: " << particleMomentum << "\n";
            std:: cout << "Orbit: " << particleOrbitType << "\n";
            std:: cout << "\n";
        }


        glfwGetFramebufferSize(window, &w, &h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform2f(uResolutionLoc, W, H);

        glBindVertexArray(VAO);
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, defaultParticle.getPosition().x, defaultParticle.getPosition().y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(defaultParticleVertices.size()));

        glBindVertexArray(trailVAO);
        glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

        glBufferData(GL_ARRAY_BUFFER, defaultTrailPositions.size() * sizeof(Vector3), defaultTrailPositions.data(), GL_DYNAMIC_DRAW);

        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, 0.0f, 0.0f);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(defaultTrailPositions.size()));

        for (auto &particle : particles) {
            glBindVertexArray(VAO);
            glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
            glUniform2f(uOffsetLoc, particle.getPosition().x, particle.getPosition().y);
            glUniform1f(uScaleLoc, 1.0f);
            glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(defaultParticleVertices.size()));

            glBindVertexArray(trailVAO);
            glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

            glBufferData(GL_ARRAY_BUFFER, vectorParticleTrails[particleIndex].size() * sizeof(Vector3), vectorParticleTrails[particleIndex].data(), GL_DYNAMIC_DRAW);

            glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
            glUniform2f(uOffsetLoc, 0.0f, 0.0f);
            glUniform1f(uScaleLoc, 1.0f);
            glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vectorParticleTrails[particleIndex].size()));

            particleIndex++;
        }

        glBindVertexArray(BHVAO);
        glUniform3f(uColorLoc, 1.0f, 0.0f, 0.0f);
        glUniform2f(uOffsetLoc, BH.getPosition().x, BH.getPosition().y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(BHVertices.size()));


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}