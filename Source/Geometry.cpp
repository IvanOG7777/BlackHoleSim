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

int getIndex (int x, int y, int z, int squaresInY, int squaresInZ) {
    return (x * (squaresInY + 1) * (squaresInZ + 1)) + (y * (squaresInZ + 1)) + z;
}

std::vector<glm::vec3> make3DGrid(int widthX, int widthY, int widthZ, int squaresInX, int squaresInY, int squaresInZ) {
    std::vector<glm::vec3> points;
    std::vector<glm:: vec3> lines;

    int deltaX = widthX / squaresInX;
    int deltaY = widthY / squaresInY;
    int deltaZ = widthZ / squaresInZ;

    for (int x = 0; x <= squaresInX; x++) {
        for (int y = 0; y <= squaresInY; y++) {
            for (int z = 0; z <= squaresInZ; z++) {
                int xVal = -(widthX / 2) + x * deltaX;
                int yVal = -(widthY / 2) + y * deltaY;
                int zVal = -(widthZ / 2) + z * deltaZ;
                points.emplace_back(xVal, yVal, zVal);
            }
        }
    }

    for (int x = 0; x < squaresInX; x++) {
        for (int y = 0; y < squaresInY; y++) {
            for (int z = 0; z < squaresInZ; z++) {

                int frontBottomLeft = getIndex(x, y, z, squaresInY, squaresInZ);
                int frontBottomRight = getIndex(x + 1, y, z, squaresInY, squaresInZ);
                int frontTopLeft = getIndex(x, y + 1, z, squaresInY, squaresInZ);
                int frontTopRight = getIndex(x + 1, y + 1, z, squaresInY, squaresInZ);

                int backBottomLeft = getIndex(x, y , z + 1, squaresInY, squaresInZ);
                int backBottomRight = getIndex(x + 1, y, z + 1, squaresInY, squaresInZ);
                int backTopLeft = getIndex(x, y + 1, z + 1, squaresInY, squaresInZ);
                int backTopRight = getIndex(x + 1, y + 1, z + 1, squaresInY, squaresInZ);

                lines.emplace_back(points[frontBottomLeft]);
                lines.emplace_back(points[frontBottomRight]);

                lines.emplace_back(points[frontBottomRight]);
                lines.emplace_back(points[frontTopRight]);

                lines.emplace_back(points[frontTopRight]);
                lines.emplace_back(points[frontTopLeft]);

                lines.emplace_back(points[frontTopLeft]);
                lines.emplace_back(points[frontBottomLeft]);


                lines.emplace_back(points[backBottomLeft]);
                lines.emplace_back(points[backBottomRight]);

                lines.emplace_back(points[backBottomRight]);
                lines.emplace_back(points[backTopRight]);

                lines.emplace_back(points[backTopRight]);
                lines.emplace_back(points[backTopLeft]);

                lines.emplace_back(points[backTopLeft]);
                lines.emplace_back(points[backBottomLeft]);


                lines.emplace_back(points[frontBottomLeft]);
                lines.emplace_back(points[backBottomLeft]);

                lines.emplace_back(points[frontBottomRight]);
                lines.emplace_back(points[backBottomRight]);

                lines.emplace_back(points[frontTopLeft]);
                lines.emplace_back(points[backTopLeft]);

                lines.emplace_back(points[frontTopRight]);
                lines.emplace_back(points[backTopRight]);
            }
        }
    }

    return lines;
}