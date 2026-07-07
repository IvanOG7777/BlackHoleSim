//
// Created by elder on 7/7/2026.
//

#include "../Header/Sphere.h"

Sphere::Sphere() {
    phase = {0,0,0};
    mesh = {};
}

void Sphere::setMesh(const std::vector<glm::vec3> &passedMesh) {
    mesh = passedMesh;
}

size_t Sphere::getMeshSize() {
    return mesh.size();
}

glm::vec3 *Sphere::getMeshData() {
    return mesh.data();
}

void Sphere::setPhase(float passedX, float passedY, float passedZ) {
    phase.x = passedX;
    phase.y = passedY;
    phase.z = passedZ;
}

void Sphere::setPhase(glm::vec3 passedPhase) {
    phase = passedPhase;
}

glm::vec3 &Sphere::getPhase() {
    return phase;
}

size_t Sphere::getTrailSize() {
    return trailPositions.size();
}

Particle::ParticleTrail *Sphere::getTrailData() {
    return trailPositions.data();
}
