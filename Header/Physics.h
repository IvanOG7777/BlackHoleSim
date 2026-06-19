//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_PHYSICS_H
#define PARTICLESIM_PHYSICS_H

#include "Particle.h"
#include "Constants.h"
void keepInFrame(Particle &particle);

Vector3 gravitationalAcceleration(const Vector3 &BHPosition, const Vector3 &particlePosition, float mu);

#endif //PARTICLESIM_PHYSICS_H