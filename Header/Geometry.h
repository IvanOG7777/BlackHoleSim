//
// Created by elder on 6/26/2026.
//

#ifndef PARTICLESIM_GEOMETRY_H
#define PARTICLESIM_GEOMETRY_H

#include <vector>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


std::vector<glm::vec3> makeUnitCircle(const float &radius);

std::vector<glm::vec3> makeGrid(int slices);

#endif //PARTICLESIM_GEOMETRY_H