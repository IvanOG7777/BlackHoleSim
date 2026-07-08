//
// Created by elder on 7/7/2026.
//

#include "../Header/SphereParticle.h"

SphereParticle::SphereParticle() {
    phase = {0,0,0};
    mesh = {};
}

void SphereParticle::setMesh(const std::vector<glm::vec3> &passedMesh) {
    mesh = passedMesh;
}

size_t SphereParticle::getMeshSize() {
    return mesh.size();
}

glm::vec3 *SphereParticle::getMeshData() {
    return mesh.data();
}

void SphereParticle::setPhase(float passedX, float passedY, float passedZ) {
    phase.x = passedX;
    phase.y = passedY;
    phase.z = passedZ;
}

void SphereParticle::setPhase(glm::vec3 passedPhase) {
    phase = passedPhase;
}

glm::vec3 &SphereParticle::getPhase() {
    return phase;
}

size_t SphereParticle::getTrailSize() {
    return trailPositions.size();
}

Particle::ParticleTrail *SphereParticle::getTrailData() {
    return trailPositions.data();
}
