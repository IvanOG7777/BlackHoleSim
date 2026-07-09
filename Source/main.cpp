#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <complex>
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

    const char *threeDVertexShader = makeVertexShader("3DVertex");
    const char *threeDVertexFragment = makeFragmentShader("3DFragment");

    GLuint threeDVS = createShader(threeDVertexShader, GL_VERTEX_SHADER);
    GLuint threeDFS = createShader(threeDVertexFragment, GL_FRAGMENT_SHADER);

    //3d shaders
    GLuint threeDShaderProgram = glCreateProgram();
    glAttachShader(threeDShaderProgram, threeDVS);
    glAttachShader(threeDShaderProgram, threeDFS);
    glLinkProgram(threeDShaderProgram);

    glDeleteShader(threeDVS);
    glDeleteShader(threeDFS);


    GLuint threeDUColorLoc = glGetUniformLocation(threeDShaderProgram, "uColor");
    GLuint uMVP = glGetUniformLocation(threeDShaderProgram, "uMVP");

    SceneState sceneState{};
    // Particle defaultParticle;

    Camera camera;
    Blackhole BH;
    SphereParticle defaultParticle;
    auto defaultParticleMesh = makeSphere(5.0f);
    defaultParticle.setMesh(defaultParticleMesh);
    // std:: vector<SphereParticle> sphereParticles;

    glm::mat4 sphereMVP;
    glm::mat4 trailMVP;
    glm::mat4 view;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), W/H, 0.1f, 1000.0f);
    glm::mat4 BHTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f));

    // sphereParticles.reserve(2000);
    // for (int i = 0; i < 2000; i++) {
    //     SphereParticle current;
    //     auto mesh = makeSphere(5.0f);
    //     current.setMesh(mesh);
    //     setDisk(BH, current);
    //     sphereParticles.emplace_back(current);
    // }

    BH.setMU(MU);
    BH.setCaptureRadius();
    BH.setPhotonSphere();
    BH.setRadius(10);
    BH.setMass(100);
    auto bhMesh = makeSphere(BH.getRadius());
    BH.setMesh(bhMesh);

    // std::vector<Particle> particles;
    // particles.reserve(2000);
    //
    // for (size_t i = 0; i < 2000; i++) {
    //     Particle particle;
    //     setDisk(BH, particle);
    //     particles.emplace_back(particle);
    // }

    setDisk(BH, defaultParticle);

    // std::vector<std::vector<Particle::ParticleTrail> > vectorParticleTrails(2000);
    // for (auto &trail : vectorParticleTrails) {
    //     trail.reserve(1000);
    // }

    std::vector<Particle::ParticleTrail> defaultTrailPositions;

    sceneState.particle = &defaultParticle;
    sceneState.blackhole = &BH;
    sceneState.trailPositions = &defaultTrailPositions;
    sceneState.camera = &camera;
    // sceneState.particles = &particles;

    glfwSetWindowUserPointer(window, &sceneState);
    glfwSetKeyCallback(window, keyCallBack);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint sphereVA0 = 0;
    GLuint sphereVBO = 0;

    // setVAO(VAO, VBO, GL_DYNAMIC_DRAW, defaultParticle.getMesh());
    setVAO(sphereVA0, sphereVBO, GL_DYNAMIC_DRAW, BH.getMesh());
    // setTrailVao(trailVAO, trailVBO, GL_DYNAMIC_DRAW, defaultTrailPositions);

    bool hasCaptured = false;
    float accumulatedTime = 0;
    float diagnosticAccumulatedTime = 0;

    glGenVertexArrays(1, &sphereVA0);
    glGenBuffers(1, &sphereVBO);
    glBindVertexArray(sphereVA0);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, BH.getMeshSize() * sizeof(glm::vec3), BH.getMeshData(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    auto startTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = glfwGetTime();
        auto deltaTime = currentTime - startTime;
        startTime = currentTime;

        accumulatedTime += static_cast<float>(deltaTime);
        diagnosticAccumulatedTime += static_cast<float>(deltaTime);

        int w = W;
        int h = H;

        glfwGetFramebufferSize(window, &w, &h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 position = camera.getPosition();
        glm::vec3 direction = camera.getDirection();
        glm::vec3 right = camera.getRight();
        camera.speed = 10.0f;
        camera.speed *= static_cast<float>(deltaTime);

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

        camera.setPosition(position);

        glUseProgram(threeDShaderProgram);

        view = camera.getViewMatrix();

        sphereMVP = projection * view * BHTranslation;
        trailMVP = projection * view * glm::mat4(1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, BH.getMeshSize() * sizeof(glm::vec3), BH.getMeshData(), GL_DYNAMIC_DRAW);
        glUniformMatrix4fv(uMVP, 1, GL_FALSE,  glm::value_ptr(sphereMVP));
        glBindVertexArray(sphereVA0);
        glUniform3f(threeDUColorLoc, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(BH.getMeshSize()));


        int particleIndex = 0;
        float bhMU = BH.getMU();
        auto bhPosition = BH.getPosition();

        // Fixed per frame time
        if (accumulatedTime >= FIXED_DT) {
            accumulatedTime -= FIXED_DT;
            deltaTime = FIXED_DT;
            defaultParticle.update(static_cast<float>(deltaTime));
            // for (auto &particle: particles) {
            //     particle.update(static_cast<float>(deltaTime));
            // }
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

        hasCaptured = hasBeenCaptured(bhPosition, defaultParticle.getPosition(), BH.getCaptureRadius());
        if (hasCaptured == true) {
            defaultTrailPositions.clear();
            setDisk(BH, defaultParticle);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}