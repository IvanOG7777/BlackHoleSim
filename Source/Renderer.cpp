//
// Created by elder on 6/19/2026.
//

#include "../Header/Renderer.h"

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
