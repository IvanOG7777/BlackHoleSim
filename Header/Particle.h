//
// Created by elder on 6/14/2026.
//

#include <iostream>
#include "Core.h"

#ifndef PARTICLESIM_PARTICLE_H
#define PARTICLESIM_PARTICLE_H


class Particle {
protected:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 forceAccumulator;

    float inverseMass;
    float mass;
    float damping;
    float kinetic;
    float radius;

    void integrate(float duration);

public:
    Particle();

    friend std:: ostream &operator<<(std::ostream stream, Vector3 &vector);

    void setPosition(Vector3 &passedPosition);
    void setPosition(float x, float y, float z);
    const Vector3 &getPosition() const;
    void printPosition();

    void setVelocity(Vector3 &passedVelocity);
    void setVelocity(float x, float y, float z);
    const Vector3 &getVelocity() const;
    void printVelocity();


    void setAcceleration(Vector3 &passedAcceleration);
    void setAcceleration(float x, float y, float z);
    const Vector3 &getAcceleration() const;
    void printAcceleration();

    void setMass(float passedMass);
    const float &getInverseMass() const;
    const float &getMass() const;

    void setDamping(float passedDamping);
    const float &getDamping() const;

    void setKinetic(float passedKinetic);
    const float &getKinetic() const;

    void setRadius(float passedRadius);
    const float getRadius() const;

    void addForce(const Vector3 &passedForce);
    void addForce(float x, float y, float z);

    void clearAccumulator();
    void clearAllValues();

    void update(float deltaTime);
};


#endif //PARTICLESIM_PARTICLE_H