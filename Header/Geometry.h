//
// Created by elder on 6/26/2026.
//

#ifndef PARTICLESIM_GEOMETRY_H
#define PARTICLESIM_GEOMETRY_H

#include <vector>
#include "Core.h"

std::vector<Vector3> makeUnitCircle(const float &radius);

std::vector<Vector3> makeGrid(int slices);

#endif //PARTICLESIM_GEOMETRY_H