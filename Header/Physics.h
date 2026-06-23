//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_PHYSICS_H
#define PARTICLESIM_PHYSICS_H

#include "Blackhole.h"
#include "Particle.h"
#include "Constants.h"
void keepInFrame(Particle &particle);

Vector3 gravitationalAcceleration(const Vector3 &BHPosition, const Vector3 &particlePosition, float mu);

bool hasBeenCaptured (const Vector3 &BHPosition, const Vector3 &particlePosition, float captureRadius);

float orbitalSpeed(const float &mu, const float &radius);

Vector3 orbitalTangent(const Vector3 &BHPosition, const Vector3 &particlePosition);

Vector3 circularVelocity(const Vector3 &BHPosition, const Vector3 &particlePosition, const float &mu, const float &radius);

float orbitalRadius(const Vector3 &BHPosition, const Vector3 &particlePosition);

void setParticleOrbit(Particle &particle, Particle &BH, const float &velocityMultiplayer);

float speed(const Vector3 &velocity);

float orbitalEnergy(const Vector3 &BHPosition, const Vector3 &particlePosition, const Vector3 &velocity, float mu);

float angularMomentum(const Vector3 &BHPosition, const Vector3 &particlePosition, const Vector3 &particleVelocity);

std:: string orbitType(const float &particleEnergy);

void setDisk(Blackhole &blackhole, Particle &particle);

#endif //PARTICLESIM_PHYSICS_H