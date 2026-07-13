//
// Created by elder on 6/14/2026.
//

#ifndef PARTICLESIM_PARTICLE_H
#define PARTICLESIM_PARTICLE_H

#include <iostream>
#include <vector>
#include <array>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


class Particle {
public:
    struct ParticleTrail {
        glm::vec3 position;
        glm::vec3 color;

        ParticleTrail();
    };

protected:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 forceAccumulator;
    ParticleTrail trail;
    std::vector<ParticleTrail> trailPositions;

    float inverseMass;
    float mass;
    float damping;
    float kinetic;
    float radius;

    void integrate(float duration);

public:
    Particle();


    friend std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vector);

    void setPosition(const glm::vec3 &passedPosition);

    void setPosition(float x, float y, float z);

    const glm::vec3 &getPosition() const;

    void printPosition() const;

    void setVelocity(const glm::vec3 &passedVelocity);

    void setVelocity(float x, float y, float z);

    const glm::vec3 &getVelocity() const;

    void printVelocity() const;


    void setAcceleration(const glm::vec3 &passedAcceleration);

    void setAcceleration(float x, float y, float z);

    const glm::vec3 &getAcceleration() const;

    void printAcceleration() const;

    void setMass(float passedMass);

    const float &getInverseMass() const;

    const float &getMass() const;

    void setDamping(float passedDamping);

    const float &getDamping() const;

    void setKinetic(float passedKinetic);

    const float &getKinetic() const;

    void setRadius(float passedRadius);

    const float &getRadius() const;

    void addForce(const glm::vec3 &passedForce);

    void addForce(float x, float y, float z);

    void clearAccumulator();

    void clearAllValues();

    void update(float deltaTime);

    void setTrail(const glm::vec3 &passedPosition, const glm::vec3 &passedColor);

    void setTrailColor(const float &particleSpeed);

    void clearTrail();

    const ParticleTrail &getTrail();

    void recordTrail(std::array<ParticleTrail, 1000> &passedPositions);
    // std::vector<ParticleTrail> &positions, const ParticleTrail &particlePosition
};


#endif //PARTICLESIM_PARTICLE_H
