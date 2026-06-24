#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "../Header/Blackhole.h"
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

const char *trailVertexShader = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPos; // Current vertex we are handling from the vector of vertices
        layout(location = 1) in vec3 trailColor; // input a vec3 color

        uniform vec2 uResolution; //holds screen W/H
        uniform vec2 uOffset; // current position of the particle
        uniform float uScale; //  particles radius

        out vec3 colorToFragment; // send out a vec3Color

        void main() {
            vec2 worldPos = aPos.xy * uScale + uOffset; // only use the xy values of the vertices, scale it and add the current position to get real new position

            vec2 ndc = worldPos / uResolution; // convert from pixel coordinated to normalized range [0,1]
            ndc = ndc * 2.0 - 1.0; // convert from normalized range to normalized device coordinates [-1,1]

            gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0); // Final position in clip space (OpenGL expects vec4 in NDC range)

            colorToFragment = trailColor; // assign so fragment can see
        }
    )GLSL";

const char *trailFragmentShader = R"GLSL(
        #version 330 core
        in vec3 colorToFragment; // color coming in from trailVertex
        out vec4 TrailColor; // Final color to output
        void main() {
            TrailColor = vec4(colorToFragment, 1.0); // set the trail vertex and alpah
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

    GLuint trailVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(trailVS, 1, &trailVertexShader, nullptr);
    glCompileShader(trailVS);
    GLuint trailFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(trailFS, 1, &trailFragmentShader, nullptr);
    glCompileShader(trailFS);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    GLuint trailShaderProgram = glCreateProgram();
    glAttachShader(trailShaderProgram, trailVS);
    glAttachShader(trailShaderProgram, trailFS);
    glLinkProgram(trailShaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(trailVS);
    glDeleteShader(trailFS);

    GLuint uResolutionLoc = glGetUniformLocation(shaderProgram, "uResolution");
    GLuint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLuint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    GLuint uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");

    GLuint tResolutionLoc = glGetUniformLocation(trailShaderProgram, "uResolution");
    GLuint tOffsetLoc = glGetUniformLocation(trailShaderProgram, "uOffset");
    GLuint tScaleLoc = glGetUniformLocation(trailShaderProgram, "uScale");

    SceneState sceneState{};
    Particle defaultParticle;
    Blackhole BH;

    BH.setMU(MU);
    BH.setCaptureRadius();
    BH.setPhotonSphere();
    BH.setPosition(960, 540, 0);
    BH.setRadius(25);
    BH.setMass(100);

    std:: vector<Particle> particles;
    particles.reserve(1000);

    for (size_t i = 0; i < 1000; i++) {
        Particle particle;
        setDisk(BH, particle);
        particles.emplace_back(particle);
    }

    for (auto &particle : particles) {
        particle.printPosition();
        particle.printVelocity();
        particle.printAcceleration();
    }

    setDisk(BH, defaultParticle);

    std:: vector<std::vector<Vector3>> vectorParticleTrails(1000);
    for (int i = 0; i < 1000; i++) {
        vectorParticleTrails[i].emplace_back(particles[i].getPosition());
    }

    std::vector<Vector3> defaultParticleVertices;
    std::vector<Vector3> BHVertices;
    std::vector<Vector3> captureRadiusVertices;
    std:: vector<Vector3> photonSphereVertices;
    std::vector<Particle::ParticleTrail> defaultTrailPositions;

    defaultParticleVertices = makeUnitCircle(defaultParticle.getRadius());
    BHVertices = makeUnitCircle(BH.getRadius());
    captureRadiusVertices = makeUnitCircle(BH.getCaptureRadius());
    photonSphereVertices = makeUnitCircle(BH.getPhotonSphere());

    sceneState.particle = &defaultParticle;
    sceneState.blackhole = &BH;
    sceneState.trailPositions = &defaultTrailPositions;
    sceneState.particles = &particles;

    glfwSetWindowUserPointer(window, &sceneState);
    glfwSetKeyCallback(window, keyCallBack);


    GLuint VAO = 0, BHVAO = 0, captureVAO = 0, photonSphereVAO = 0, trailVAO = 0;
    GLuint VBO = 0, BHVBO = 0, captureVBO = 0, photonSphereVBO = 0, trailVBO = 0;

    setVAO(VAO, VBO, GL_DYNAMIC_DRAW, defaultParticleVertices);
    setVAO(BHVAO, BHVBO, GL_DYNAMIC_DRAW, BHVertices);
    setVAO(captureVAO, captureVBO, GL_DYNAMIC_DRAW, captureRadiusVertices);
    setVAO(photonSphereVAO, photonSphereVBO, GL_DYNAMIC_DRAW, photonSphereVertices);
    setTrailVao(trailVAO, trailVBO, GL_DYNAMIC_DRAW, defaultTrailPositions);

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
        float bhMU = BH.getMU();
        auto bhPosition = BH.getPosition();

        if (accumulatedTime >= FIXED_DT) {
            accumulatedTime -= FIXED_DT;
            dt = FIXED_DT;
            defaultParticle.update(dt);
            // for (auto &particle : particles) {
            //     particle.update(dt);
            // }
        }
        Vector3 acceleration = gravitationalAcceleration(bhPosition, defaultParticle.getPosition(), bhMU);
        defaultParticle.setAcceleration(acceleration);
        defaultParticle.setTrail(defaultParticle.getPosition(), {1.0f, 1.0f, 1.0f});
        recordTrail(defaultTrailPositions, defaultParticle.getTrail());

        // for (auto &particle : particles) {
        //     Vector3 acc = gravitationalAcceleration(bhPosition, particle.getPosition(), bhMU);
        //     particle.setAcceleration(acc);
        //     recordTrail(vectorParticleTrails[particleIndex], particle.getPosition());
        //     particleIndex++;
        // }

        particleIndex = 0;

        hasCaptured = hasBeenCaptured(bhPosition, defaultParticle.getPosition(), BH.getCaptureRadius());
        if (hasCaptured == true) {

        }

        float particleRadius = orbitalRadius(bhPosition, defaultParticle.getPosition());
        float particleSpeed = speed(defaultParticle.getVelocity());
        float particleEnergy = orbitalEnergy(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity(), bhMU);
        float particleMomentum = angularMomentum(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity());
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

        // for (auto &particle : particles) {
        //     glBindVertexArray(VAO);
        //     glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
        //     glUniform2f(uOffsetLoc, particle.getPosition().x, particle.getPosition().y);
        //     glUniform1f(uScaleLoc, 1.0f);
        //     glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(defaultParticleVertices.size()));
        //
        //     glBindVertexArray(trailVAO);
        //     glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
        //
        //     glBufferData(GL_ARRAY_BUFFER, vectorParticleTrails[particleIndex].size() * sizeof(Vector3), vectorParticleTrails[particleIndex].data(), GL_DYNAMIC_DRAW);
        //
        //     glUniform3f(uColorLoc, 0.45f, 0.090f, 0.0f);
        //     glUniform2f(uOffsetLoc, 0.0f, 0.0f);
        //     glUniform1f(uScaleLoc, 1.0f);
        //     glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vectorParticleTrails[particleIndex].size()));
        //
        //     particleIndex++;
        // }

        glBindVertexArray(captureVAO);
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, bhPosition.x, bhPosition.y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(captureRadiusVertices.size()));

        glBindVertexArray(photonSphereVAO);
        glUniform3f(uColorLoc, 1.0f, 0.5f, 1.0f);
        glUniform2f(uOffsetLoc, bhPosition.x, bhPosition.y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(photonSphereVertices.size()));

        glBindVertexArray(BHVAO);
        glUniform3f(uColorLoc, 1.0f, 0.0f, 0.0f);
        glUniform2f(uOffsetLoc, bhPosition.x, bhPosition.y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(BHVertices.size()));

        glUseProgram(trailShaderProgram);
        glUniform2f(tResolutionLoc, W, H);

        glBindVertexArray(trailVAO);
        glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

        glBufferData(GL_ARRAY_BUFFER, defaultTrailPositions.size() * sizeof(Particle::ParticleTrail), defaultTrailPositions.data(), GL_DYNAMIC_DRAW);

        glUniform2f(tOffsetLoc, 0.0f, 0.0f);
        glUniform1f(tScaleLoc, 1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(defaultTrailPositions.size()));


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}