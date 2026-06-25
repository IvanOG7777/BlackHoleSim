//
// Created by elder on 6/19/2026.
//

#include "../Header/Renderer.h"

#include "glad/glad.h"

std::vector<Vector3> makeUnitCircle(const float &radius) {
    std::vector<Vector3> vertices;
    int segments = 64;
    vertices.reserve(segments + 2);
    // vertices.emplace_back(0,0,0);

    for (int i = 0; i <= segments; i++) {
        float progress = static_cast<float>(i) / segments;
        float theta = progress * 2.0f * PI;

        Vector3 position;

        position.x = 0 + std::cos(theta) * radius;
        position.y = 0 + std::sin(theta) * radius;
        position.z = 0;

        vertices.emplace_back(position);
    }

    return vertices;
}

std::vector<std::vector<Vector3>> makeGrid(int slices) {
    std:: vector<std::vector<Vector3>> coordinates;
    std:: vector<Vector3> vertices;
    std:: vector<Vector3> indices;

    for (int row = 0; row <= slices; row++) {
        for (int col = 0; col <= slices; col++) {

            float x = static_cast<float>(row) / static_cast<float>(slices) * H;
            float y = static_cast<float>(col) / static_cast<float>(slices) * W;

            vertices.emplace_back(x, y, 0);
        }
    }

    for (int row = 0; row <= slices; row++) {
        for (int col = 0; col <= slices; col++) {
            GLuint bottomLeft = row * (slices + 1) + col;
            GLuint bottomRight = row * (slices + 1) + col + 1;
            GLuint topLeft = (row + 1) * (slices + 1) + col;
            GLuint topRight = (row + 1) * (slices + 1) + col + 1;

            indices.emplace_back(bottomLeft, bottomRight, topLeft);
            indices.emplace_back(bottomRight, topRight, topLeft);
        }
    }

    coordinates.emplace_back(vertices);
    coordinates.emplace_back(indices);

    return coordinates;
}

void recordTrail(std::vector<Particle::ParticleTrail> &trailPositions, const Particle::ParticleTrail &particlePosition) {
    if (trailPositions.size() >= 1000) { // if size is greater than 100 position
        trailPositions.erase(trailPositions.begin()); // delete stale position
        trailPositions.emplace_back(particlePosition); // place in newest positon
        return;
    }
    trailPositions.emplace_back(particlePosition); // keep placing until 100
}

void setVAO(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<Vector3> &vector3s) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vector3s.size() * sizeof(Vector3), vector3s.data(), drawHint);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void *)0);
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

const char *createVertexShader(const std::string shaderType) {
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

    return "";
}

const char *createFragmentShader(const std::string& shaderType) {
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
    return "";
}