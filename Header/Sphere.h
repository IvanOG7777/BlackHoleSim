//
// Created by elder on 7/7/2026.
//

#ifndef PARTICLESIM_SPHERE_H
#define PARTICLESIM_SPHERE_H

#include <vector>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Particle.h"

class Sphere : public Particle {
private:
    glm::vec3 phase;
    std::vector<glm::vec3> mesh;

public:

    Sphere();

    void setMesh(const std::vector<glm::vec3> &passedMesh);
    size_t getMeshSize();
    glm::vec3 *getMeshData();

    void setPhase(float passedX, float passedY, float passedZ);
    void setPhase(glm::vec3 passedPhase);
    glm::vec3 &getPhase();

    size_t getTrailSize();
    ParticleTrail *getTrailData();

};

#endif //PARTICLESIM_SPHERE_H