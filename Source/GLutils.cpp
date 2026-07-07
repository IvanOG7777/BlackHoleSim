//
// Created by elder on 6/26/2026.
//

#include "../Header/GLutils.h"

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

void setTrailVao(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<Particle::ParticleTrail> &trails) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, trails.size() * sizeof(Particle::ParticleTrail), trails.data(), drawHint);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::ParticleTrail), (void *)0); //position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle::ParticleTrail), (void *)offsetof(Particle::ParticleTrail, color)); //color // skip over pos(16 bytes) to get to color Vector3 within ParticleTrail struct
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

const char *makeVertexShader(const std::string &shaderType) {
    if (shaderType == "CircleVertex") {
        return R"GLSL(
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
    }

    if (shaderType == "TrailVertex") {
        return R"GLSL(
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
    }

    if (shaderType == "GridVertex") {
        return "";
    }

    return "";
}

const char *makeFragmentShader(const std::string& shaderType) {
    if (shaderType == "CircleFragment") {
        return R"GLSL(
            #version 330 core
            out vec4 FragColor; // color that will be sent out to screen
            uniform vec3 uColor; // RGB
            void main() {
                FragColor = vec4(uColor, 1.0); // pass in the RBG plus alpha
            }
        )GLSL";
    }

    if (shaderType == "TrailFragment") {
        return R"GLSL(
            #version 330 core
            in vec3 colorToFragment; // color coming in from trailVertex
            out vec4 TrailColor; // Final color to output
            void main() {
                TrailColor = vec4(colorToFragment, 1.0); // set the trail vertex and alpah
            }
        )GLSL";
    }

    if (shaderType == "GridFragment") {
        return "";
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

