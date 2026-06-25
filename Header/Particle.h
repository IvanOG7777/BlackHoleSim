//
// Created by elder on 6/14/2026.
//

#ifndef PARTICLESIM_PARTICLE_H
#define PARTICLESIM_PARTICLE_H

#include <iostream>
#include "Core.h"


class Particle {
public:
    struct ParticleTrail {
        Vector3 position;
        Vector3 color;

        ParticleTrail();
    };

protected:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 forceAccumulator;
    ParticleTrail trail;

    float inverseMass;
    float mass;
    float damping;
    float kinetic;
    float radius;

    void integrate(float duration);

public:
    Particle();


    friend std::ostream &operator<<(std::ostream &stream, const Vector3 &vector);

    void setPosition(const Vector3 &passedPosition);

    void setPosition(float x, float y, float z);

    const Vector3 &getPosition() const;

    void printPosition() const;

    void setVelocity(const Vector3 &passedVelocity);

    void setVelocity(float x, float y, float z);

    const Vector3 &getVelocity() const;

    void printVelocity() const;


    void setAcceleration(const Vector3 &passedAcceleration);

    void setAcceleration(float x, float y, float z);

    const Vector3 &getAcceleration() const;

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

    void addForce(const Vector3 &passedForce);

    void addForce(float x, float y, float z);

    void clearAccumulator();

    void clearAllValues();

    void update(float deltaTime);

    void setTrail(const Vector3 &passedPosition, const Vector3 &passedColor);

    void setTrailColor(const float &particleSpeed);

    void clearTrail();

    const ParticleTrail &getTrail();
};


#endif //PARTICLESIM_PARTICLE_H
