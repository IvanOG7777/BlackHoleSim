//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_PHYSICS_H
#define PARTICLESIM_PHYSICS_H

#include "Blackhole.h"
#include "Particle.h"
#include "Constants.h"
void keepInFrame(Particle &particle);

glm::vec3 gravitationalAcceleration(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, float mu);

bool hasBeenCaptured (const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, float captureRadius);

float orbitalSpeed(const float &mu, const float &radius);

glm::vec3 orbitalTangent(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition);

glm::vec3 circularVelocity(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, const float &mu, const float &radius);

float orbitalRadius(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition);

void setParticleOrbit(Particle &particle, Particle &BH, const float &velocityMultiplayer);

float speed(const glm::vec3&velocity);

float orbitalEnergy(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, const glm::vec3 &velocity, float mu);

float angularMomentum(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, const glm::vec3 &particleVelocity);

std:: string orbitType(const float &particleEnergy);

void setDisk(Blackhole &blackhole, Particle &particle);

#endif //PARTICLESIM_PHYSICS_H