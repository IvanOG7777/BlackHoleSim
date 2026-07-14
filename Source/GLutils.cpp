//
// Created by elder on 6/26/2026.
//

#include "../Header/GLutils.h"

#include <array>

void setVAO(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<glm::vec3> &vector3s) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vector3s.size() * sizeof(glm::vec3), vector3s.data(), drawHint);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void setTrailVao(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::array<Particle::ParticleTrail, 1000> &trails) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, trails.size() * sizeof(Particle::ParticleTrail), trails.data(), drawHint);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::ParticleTrail), (void *)0); //position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::ParticleTrail), (void*)offsetof(Particle::ParticleTrail, color));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

const char *makeVertexShader(const std::string &shaderType) {
    if (shaderType == "3DVertex") {
        return R"GLSL(
            #version 330 core
            layout(location = 0) in vec3 aPos;
            layout(location = 1) in vec3 aColor;

            uniform mat4 uMVP;
            uniform bool uUseVertexColor;
            uniform vec3 uColor;

            out vec3 vertexColor;

            void main() {
                gl_Position = uMVP * vec4(aPos, 1.0);

                if (uUseVertexColor) {
                    vertexColor = aColor;
                } else {
                    vertexColor = uColor;
                }
            }
        )GLSL";
    }

    return "";
}

const char *makeFragmentShader(const std::string& shaderType) {
    if (shaderType == "3DFragment") {
        return R"GLSL(
           #version 330 core

            in vec3 vertexColor;
            out vec4 fragment;

            void main() {
                fragment = vec4(vertexColor, 1.0);
            }
        )GLSL";
    }
    return "";
}

GLuint createShader(const char *shader, GLenum shaderType) {
    GLuint s = glCreateShader(shaderType);
    glShaderSource(s, 1, &shader, nullptr);
    glCompileShader(s);

    return s;
}

GLFWwindow *createWindow(int width, int height, const char *windowName) {
    if (width == 0 || height == 0) {
        std:: cerr << "BAD WINDOW WIDTH OR HEIGHT" << std:: endl;
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "WINDOW ERROR, COULDN'T CREATE, WINDOW IS NULL\n";
        glfwTerminate();
        std:: cerr << "WINDOW IS NULLPTR" << std:: endl;
        exit(EXIT_FAILURE);
    }

    return window;
}

