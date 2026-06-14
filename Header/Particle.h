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

    void setPosition(Vector3 &position);
    void setPosition(float x, float y, float z);
    const Vector3 &getPosition();
    void printPosition();

    void setVelocity(Vector3 &velocity);
    void setVelocity(float x, float y, float z);
    const Vector3 &getVelocity();
    void printVelocity();


    void setAcceleration(Vector3 &acceleration);
    void setAcceleration(float x, float y, float z);
    const Vector3 &getAcceleration();
    void printAcceleration();

    void setMass(float mass);
    const float &getInverseMass() const;
    const float &getMass() const;


};


#endif //PARTICLESIM_PARTICLE_H