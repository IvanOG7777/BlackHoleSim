#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

#include "../Header/Particle.h"

float COR = .65f;
float yVelThreshold = 0.2f;
constexpr float PI = 3.14159265358979323846f;
constexpr float W = 1280.0f;
constexpr float H = 720.0f;

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

void keepInFrame(Particle &particle, int &w, int &h) {
    float radius = particle.getRadius();

    float minX = radius;
    float maxX = static_cast<float>(w) - radius;
    float minY = radius;
    float maxY = static_cast<float>(h) - radius;

    Vector3 pos = particle.getPosition();
    Vector3 vel = particle.getVelocity();

    if (pos.x < minX) {
        pos.x = minX;
        vel.x = -vel.x * COR;
    }

    if (pos.x > maxX) {
        pos.x = maxX;
        vel.x = - vel.x * COR;
    }

    if (pos.y < minY) {
        pos.y = minY;
        vel.y = -vel.y * COR;
    }

    if (pos.y > maxY) {
        pos.y = maxY;
        vel.y = - vel.y * COR;
    }

    particle.setPosition(pos);
    particle.setVelocity(vel);
}

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

    bool isStill = false;
    Particle particle;
    particle.printPosition();
    particle.printVelocity();
    particle.printAcceleration();

    particle.setPosition(200, 200, 0);
    particle.setVelocity(1, 0, 0);
    particle.setAcceleration(0, -980, 0);
    particle.setDamping(1.0f);
    particle.setMass(5);
    particle.setRadius(10);
    particle.setKinetic(5);

    int loopCounter = 0;

    std:: vector<Vector3> circleVertices;
    Vector3 center;
    int segments = 64;
    circleVertices.reserve(segments + 2);
    circleVertices.emplace_back(center);

    for (int i = 0; i <= segments; i++) {
        float progress = static_cast<float>(i) / static_cast<float>(segments);
        float theta = progress * 2.0f * PI;

        Vector3 position;

        position.x = center.x + std::cos(theta) * particle.getRadius();
        position.y = center.y + std::sin(theta) * particle.getRadius();
        position.z = 0;

        circleVertices.emplace_back(position);
    }

    int slices = 15;
    std:: vector<Vector3> vertices;
    std:: vector<Vector3> indices;

    for (size_t row = 0; row <= slices; row++) {
        for (size_t col = 0; col <= slices; col++) {

            float x = static_cast<float>(row) / static_cast<float>(slices) * H;
            float y = static_cast<float>(col) / static_cast<float>(slices) * W;
            GLfloat z = 0;

            Vector3 newVec3(x,y,z);
            std:: cout << newVec3;
            vertices.push_back(newVec3);
        }
    }

    for (size_t row = 0; row <= slices; row++) {
        for (size_t col = 0; col <= slices; col++) {
            GLuint bottomLeft = row * (slices + 1) + col;
            GLuint bottomRight = row * (slices + 1) + col + 1;
            GLuint topLeft = (row + 1) * (slices + 1) + col;
            GLuint topRight = (row + 1) * (slices + 1) + col + 1;

            indices.emplace_back(bottomLeft, bottomRight, topLeft);
            indices.emplace_back(bottomRight, topRight, topLeft);
        }
    }

    GLuint VAO = 0;
    GLuint VBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,circleVertices.size() * sizeof(Vector3),circleVertices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    auto startTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - startTime;
        startTime = currentTime;
        const auto dt = deltaTime.count();

        int w = W;
        int h = H;

        glfwGetFramebufferSize(window, &w, &h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform2f(uResolutionLoc, W, H);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        if (isStill == false) {
            particle.update(dt);

            keepInFrame(particle, w, h);

            if (particle.getPosition().y == particle.getRadius() && particle.getVelocity().y <= 0.0f) isStill = true;
            particle.printPosition();
            particle.printVelocity();
            particle.printAcceleration();
            std:: cout << "Delta time: " << dt << std::endl;
            loopCounter++;
            std::cout << std::endl;
        }

        glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
        glUniform2f(uOffsetLoc, particle.getPosition().x, particle.getPosition().y);
        glUniform1f(uScaleLoc, 1.0f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(circleVertices.size()));

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Loops done: " << loopCounter << std::endl;

    return 0;
}
