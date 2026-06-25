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

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit()) {
        std::cerr << "GLFW INIT ERROR \n";
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(W, H, "Black Hole Simulation", NULL, NULL);
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

    const char *vertexShader = makeVertexShader("CircleVertex");
    const char *fragmentShader = makeFragmentShader("CircleFragment");

    const char *trailVertexShader = makeVertexShader("TrailVertex");
    const char *trailFragmentShader = makeFragmentShader("TrailFragment");

    GLuint vs = createShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = createShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLuint trailVS = createShader(trailVertexShader, GL_VERTEX_SHADER);
    GLuint trailFS = createShader(trailFragmentShader, GL_FRAGMENT_SHADER);

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

    std::vector<Particle> particles;
    particles.reserve(2000);

    for (size_t i = 0; i < 2000; i++) {
        Particle particle;
        setDisk(BH, particle);
        particles.emplace_back(particle);
    }

    setDisk(BH, defaultParticle);

    std::vector<std::vector<Particle::ParticleTrail> > vectorParticleTrails(2000);

    std::vector<Vector3> defaultParticleVertices;
    std::vector<Vector3> BHVertices;
    std::vector<Vector3> captureRadiusVertices;
    std::vector<Vector3> photonSphereVertices;
    std::vector<Vector3> triangles;
    std::vector<Particle::ParticleTrail> defaultTrailPositions;

    defaultParticleVertices = makeUnitCircle(defaultParticle.getRadius());
    BHVertices = makeUnitCircle(BH.getRadius());
    captureRadiusVertices = makeUnitCircle(BH.getCaptureRadius());
    photonSphereVertices = makeUnitCircle(BH.getPhotonSphere());
    triangles = makeGrid(3);

    sceneState.particle = &defaultParticle;
    sceneState.blackhole = &BH;
    sceneState.trailPositions = &defaultTrailPositions;
    sceneState.particles = &particles;

    glfwSetWindowUserPointer(window, &sceneState);
    glfwSetKeyCallback(window, keyCallBack);


    GLuint VAO = 0, BHVAO = 0, captureVAO = 0, photonSphereVAO = 0, trailVAO = 0, gridVAO = 0;
    GLuint VBO = 0, BHVBO = 0, captureVBO = 0, photonSphereVBO = 0, trailVBO = 0, gridVBO = 0;

    setVAO(VAO, VBO, GL_DYNAMIC_DRAW, defaultParticleVertices);
    setVAO(BHVAO, BHVBO, GL_DYNAMIC_DRAW, BHVertices);
    setVAO(captureVAO, captureVBO, GL_DYNAMIC_DRAW, captureRadiusVertices);
    setVAO(photonSphereVAO, photonSphereVBO, GL_DYNAMIC_DRAW, photonSphereVertices);
    setVAO(gridVAO, gridVBO, GL_DYNAMIC_DRAW, triangles);
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
            for (auto &particle: particles) {
                particle.update(dt);
            }
        }

        float particleRadius = orbitalRadius(bhPosition, defaultParticle.getPosition());
        float particleSpeed = speed(defaultParticle.getVelocity());
        float particleEnergy = orbitalEnergy(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity(), bhMU);
        float particleMomentum = angularMomentum(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity());
        std::string particleOrbitType = orbitType(particleEnergy);

        Vector3 acceleration = gravitationalAcceleration(bhPosition, defaultParticle.getPosition(), bhMU);
        defaultParticle.setAcceleration(acceleration);
        defaultParticle.setTrailColor(particleSpeed);

        recordTrail(defaultTrailPositions, defaultParticle.getTrail());

        particleIndex = 0;
        for (auto &particle: particles) {
            Vector3 acc = gravitationalAcceleration(bhPosition, particle.getPosition(), bhMU);
            particle.setAcceleration(acc);

            particleSpeed = speed(particle.getVelocity());
            particle.setTrailColor(particleSpeed);

            recordTrail(vectorParticleTrails[particleIndex], particle.getTrail());
            particleIndex++;
        }

        hasCaptured = hasBeenCaptured(bhPosition, defaultParticle.getPosition(), BH.getCaptureRadius());
        if (hasCaptured == true) {
            defaultTrailPositions.clear();
            setDisk(BH, defaultParticle);
        }

        particleIndex = 0;
        for (auto &particle : particles) {
            hasCaptured = hasBeenCaptured(bhPosition, particle.getPosition(), BH.getCaptureRadius());
            if (hasCaptured == true) {
                vectorParticleTrails[particleIndex].clear();
                setDisk(BH, particle);
            }
            particleIndex++;
        }

        if (diagnosticAccumulatedTime >= DIAGNOSTIC_TIME) {
            diagnosticAccumulatedTime -= DIAGNOSTIC_TIME;
            std::cout << "Radius: " << particleRadius << "\n";
            std::cout << "Speed: " << particleSpeed << "\n";
            std::cout << "Energy: " << particleEnergy << "\n";
            std::cout << "Momentum: " << particleMomentum << "\n";
            std::cout << "Orbit: " << particleOrbitType << "\n";
            std::cout << "\n";
        }


        glfwGetFramebufferSize(window, &w, &h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform2f(uResolutionLoc, W, H);

        glBindVertexArray(gridVAO);
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, 0.0f, 0.0f);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(triangles.size()));

        glBindVertexArray(VAO);
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform2f(uOffsetLoc, defaultParticle.getPosition().x, defaultParticle.getPosition().y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(defaultParticleVertices.size()));

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

        particleIndex = 0;
        for (auto &particle : particles) {
            glBindVertexArray(trailVAO);
            glBindBuffer(GL_ARRAY_BUFFER, trailVBO);

            glBufferData(GL_ARRAY_BUFFER, vectorParticleTrails[particleIndex].size() * sizeof(Particle::ParticleTrail), vectorParticleTrails[particleIndex].data(), GL_DYNAMIC_DRAW);

            glUniform2f(tOffsetLoc, 0.0f, 0.0f);
            glUniform1f(tScaleLoc, 1.0f);
            glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vectorParticleTrails[particleIndex].size()));

            particleIndex++;
        }


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}