//
// Created by elder on 6/19/2026.
//

#include "../Header/Renderer.h"

#include "glad/glad.h"

std::vector<Vector3> makeUnitCircle(const float &radius) {
    std::vector<Vector3> vertices;
    int segments = 64;
    vertices.reserve(segments + 2);
    vertices.emplace_back(0,0,0);

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

            Vector3 tri1(bottomLeft, bottomRight, topLeft);
            Vector3 tri2()
        }
    }
}
