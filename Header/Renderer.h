//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_RENDERER_H
#define PARTICLESIM_RENDERER_H

#include <vector>
#include "Core.h"
#include "../Header/Constants.h"

std:: vector<Vector3> makeUnitCircle(const float &radius);

std:: vector<std::vector<Vector3>> makeGrid(int slices);

void recordTrail(std:: vector<Vector3> &positions, const Vector3 &particlePosition);

void drawTrail();

#endif //PARTICLESIM_RENDERER_H