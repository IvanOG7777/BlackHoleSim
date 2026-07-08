//
// Created by elder on 6/26/2026.
//


#include "glad/glad.h"
#include "../Header/Geometry.h"
#include "../Header/Constants.h"

std::vector<glm::vec3> makeUnitCircle(const float &radius) {
    std::vector<glm::vec3> vertices;
    int segments = 64;
    vertices.reserve(segments + 2);
    // vertices.emplace_back(0,0,0);

    for (int i = 0; i <= segments; i++) {
        float progress = static_cast<float>(i) / static_cast<float>(segments);
        float theta = progress * 2.0f * PI;

        glm::vec3 position;

        position.x = 0 + std::cos(theta) * radius;
        position.y = 0 + std::sin(theta) * radius;
        position.z = 0;

        vertices.emplace_back(position);
    }

    return vertices;
}

std::vector<glm::vec3> makeGrid(int slices) {
    std:: vector<glm::vec3> points;
    std:: vector<glm::vec3> triangles;

    // minimum reserve size to avoid too many allocation
    points.reserve(slices * slices);
    triangles.reserve(slices * slices);

    for (int row = 0; row <= slices; row++) {
        for (int col = 0; col <= slices; col++) {

            float x = static_cast<float>(row) / static_cast<float>(slices) * W;
            float y = static_cast<float>(col) / static_cast<float>(slices) * H;

            points.emplace_back(x, y, 0);
        }
    }

    for (int row = 0; row < slices; row++) {
        for (int col = 0; col < slices; col++) {
            GLuint bottomLeft = row * (slices + 1) + col;
            GLuint bottomRight = row * (slices + 1) + col + 1;
            GLuint topLeft = (row + 1) * (slices + 1) + col;
            GLuint topRight = (row + 1) * (slices + 1) + col + 1;

            //triangle one
            triangles.emplace_back(points[bottomLeft]);
            triangles.emplace_back(points[topLeft]);
            triangles.emplace_back(points[bottomRight]);

            //triangle 2
            triangles.emplace_back(points[bottomLeft]);
            triangles.emplace_back(points[bottomRight]);
            triangles.emplace_back(points[topRight]);
        }
    }

    return triangles;
}

std::vector<glm::vec3> makeSphere(float radius) {
    std:: vector<glm::vec3> points;
    std::vector<glm::vec3> triangles;

    int stacks = 16; // rows
    int sectors = 32; // cols

    // minimum reserve size to avoid too many allocation
    points.reserve(stacks * sectors);
    triangles.reserve(stacks * sectors);

    for (int row = 0; row <= stacks; row++) {
        for (int col = 0; col <= sectors; col++) {
            float phi = glm::pi<float>() * static_cast<float>(row) / static_cast<float>(stacks);
            float theta = (2.0f * glm::pi<float>()) * static_cast<float>(col) / static_cast<float>(sectors);

            float x = std:: cosf(theta) * std:: sinf(phi);
            float y = std:: cosf(phi);
            float z = std:: sinf(theta) * std:: sinf(phi);

            points.emplace_back(x * radius, y * radius, z * radius);
        }
    }

    for (int row = 0; row < stacks; row++) {
        for (int col = 0; col < sectors; col++) {
            int bottomLeft = row * (sectors + 1) + col;
            int bottomRight = row * (sectors + 1) + col + 1;
            int topLeft = (row + 1) * (sectors + 1) + col;
            int topRight = (row + 1) * (sectors + 1) + col + 1;

            triangles.emplace_back(points[bottomLeft]);
            triangles.emplace_back(points[topLeft]);
            triangles.emplace_back(points[bottomRight]);

            triangles.emplace_back(points[bottomRight]);
            triangles.emplace_back(points[topLeft]);
            triangles.emplace_back(points[topRight]);
        }
    }

    return triangles;
}