#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "../Header/Blackhole.h"
#include "../Header/Camera.h"
#include "../Header/Constants.h"
#include "../Header/Particle.h"
#include "../Header/Physics.h"
#include "../Header/Input.h"
#include "../Header/Geometry.h"
#include "../Header/GLutils.h"

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit()) {
        std::cerr << "GLFW INIT ERROR \n";
        return -1;
    }

    GLFWwindow *window = createWindow(W, H, "Black hole simulation");

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "GLAD INIT ERROR\n";
        return -1;
    }

    const char *vertexShader = makeVertexShader("CircleVertex");
    const char *fragmentShader = makeFragmentShader("CircleFragment");

    const char *trailVertexShader = makeVertexShader("TrailVertex");
    const char *trailFragmentShader = makeFragmentShader("TrailFragment");

    const char *threeDVertexShader = makeVertexShader("3DVertex");
    const char *threeDVertexFragment = makeVertexShader("3DFragment");

    GLuint vs = createShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = createShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLuint trailVS = createShader(trailVertexShader, GL_VERTEX_SHADER);
    GLuint trailFS = createShader(trailFragmentShader, GL_FRAGMENT_SHADER);

    GLuint threeDVS = createShader(threeDVertexShader, GL_VERTEX_SHADER);
    GLuint threeDFS = createShader(threeDVertexFragment, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    GLuint trailShaderProgram = glCreateProgram();
    glAttachShader(trailShaderProgram, trailVS);
    glAttachShader(trailShaderProgram, trailFS);
    glLinkProgram(trailShaderProgram);

    GLuint threeDShaderProgram = glCreateProgram();
    glAttachShader(threeDShaderProgram, threeDVS);
    glAttachShader(threeDShaderProgram, threeDFS);
    glLinkProgram(threeDShaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(trailVS);
    glDeleteShader(trailFS);
    glDeleteShader(threeDVS);
    glDeleteShader(threeDFS);

    GLuint uResolutionLoc = glGetUniformLocation(shaderProgram, "uResolution");
    GLuint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLuint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    GLuint uScaleLoc = glGetUniformLocation(shaderProgram, "uScale");

    GLuint tResolutionLoc = glGetUniformLocation(trailShaderProgram, "uResolution");
    GLuint tOffsetLoc = glGetUniformLocation(trailShaderProgram, "uOffset");
    GLuint tScaleLoc = glGetUniformLocation(trailShaderProgram, "uScale");

    GLuint threeDUColorLoc = glGetUniformLocation(threeDShaderProgram, "uColor");
    GLuint uMVP = glGetUniformLocation(threeDShaderProgram, "uMVP");

    SceneState sceneState{};
    Particle defaultParticle;

    Camera camera;
    Blackhole BH;
    SphereParticle defaultSphere;
    std:: vector<SphereParticle> sphereParticles;

    sphereParticles.reserve(2000);
    for (int i = 0; i < 2000; i++) {
        SphereParticle current;
        auto mesh = makeSphere(5.0f);
        current.setMesh(mesh);
        setDisk(BH, current);
        sphereParticles.emplace_back(current);
    }

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
    for (auto &trail : vectorParticleTrails) {
        trail.reserve(1000);
    }

    std::vector<glm::vec3> defaultParticleVertices;
    std::vector<glm::vec3> BHVertices;
    std::vector<glm::vec3> captureRadiusVertices;
    std::vector<glm::vec3> photonSphereVertices;
    std::vector<glm::vec3> triangles;
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

    auto startTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - startTime;
        startTime = currentTime;

        accumulatedTime += static_cast<float>(deltaTime);
        diagnosticAccumulatedTime += static_cast<float>(deltaTime);

        int w = W;
        int h = H;

        auto position = camera.getPosition();
        auto direction = camera.getDirection();
        auto right = camera.getRight();
        camera.speed = 15.0f;

        int wKey = glfwGetKey(window, GLFW_KEY_W);
        int aKey = glfwGetKey(window, GLFW_KEY_A);
        int sKey = glfwGetKey(window, GLFW_KEY_S);
        int dKey = glfwGetKey(window, GLFW_KEY_D);
        int spaceKey = glfwGetKey(window, GLFW_KEY_SPACE);
        int leftControlKey = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
        int leftShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

        if (wKey == GLFW_PRESS) {
            position += direction * camera.speed;
        }
        if (sKey == GLFW_PRESS) {
            position -= direction * camera.speed;
        }
        if (dKey == GLFW_PRESS) {
            position += right * camera.speed;
        }
        if (aKey== GLFW_PRESS) {
            position -= right * camera.speed;
        }
        if (spaceKey == GLFW_PRESS) {
            position.y += camera.speed;
        }
        if (leftControlKey == GLFW_PRESS) {
            position.y -= camera.speed;
        }

        if (wKey == GLFW_PRESS && leftShift == GLFW_PRESS) {
            position += direction *  (camera.speed * 2);
        }
        if (sKey == GLFW_PRESS && leftShift == GLFW_PRESS) {
            position -= direction *  (camera.speed * 2);
        }
        if (dKey == GLFW_PRESS && leftShift == GLFW_PRESS) {
            position += right *  (camera.speed * 2);
        }
        if (aKey== GLFW_PRESS && leftShift == GLFW_PRESS) {
            position -= right * (camera.speed * 2);
        }
        if (spaceKey == GLFW_PRESS && leftShift == GLFW_PRESS) {
            position.y += camera.speed * 2;
        }
        if (leftControlKey == GLFW_PRESS && leftShift == GLFW_PRESS) {
            position.y -= camera.speed * 2;
        }
        // keyboard polling


        int particleIndex = 0;
        float bhMU = BH.getMU();
        auto bhPosition = BH.getPosition();

        // Fixed per frame time
        if (accumulatedTime >= FIXED_DT) {
            accumulatedTime -= FIXED_DT;
            deltaTime = FIXED_DT;
            defaultParticle.update(static_cast<float>(deltaTime));
            for (auto &particle: particles) {
                particle.update(static_cast<float>(deltaTime));
            }
        }

        float particleRadius = orbitalRadius(bhPosition, defaultParticle.getPosition());
        float particleSpeed = speed(defaultParticle.getVelocity());
        float particleEnergy = orbitalEnergy(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity(), bhMU);
        float particleMomentum = angularMomentum(bhPosition, defaultParticle.getPosition(), defaultParticle.getVelocity());
        std::string particleOrbitType = orbitType(particleEnergy);

        glm::vec3 acceleration = gravitationalAcceleration(bhPosition, defaultParticle.getPosition(), bhMU);
        defaultParticle.setAcceleration(acceleration);
        defaultParticle.setTrailColor(particleSpeed);

        defaultParticle.recordTrail(defaultTrailPositions);

        particleIndex = 0;
        for (auto &particle: particles) {
            glm::vec3 acc = gravitationalAcceleration(bhPosition, particle.getPosition(), bhMU);
            particle.setAcceleration(acc);

            particleSpeed = speed(particle.getVelocity());
            particle.setTrailColor(particleSpeed);

            particle.recordTrail(vectorParticleTrails[particleIndex]);
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
        for (auto &unusedParticle : particles) {
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