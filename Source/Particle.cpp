//
// Created by elder on 6/14/2026.
//

#include <cassert>
#include "../Header/Particle.h"

Particle::Particle() {
    position = {};
    velocity = {};
    acceleration = {};
    forceAccumulator = {};

    inverseMass = 0.0f;
    mass = 0.0f;
    damping = 0.0f;
    kinetic = 0.0f;
    radius = 0.0f;
}

Particle::ParticleTrail::ParticleTrail() {
    position = {0.0f,0.0f,0.0f};
    color = {1.0f, 1.0f, 1.0f};
}

void Particle::integrate(float duration) {
    if (inverseMass <= 0.0f) return;

    assert(duration > 0.0f);

    glm::vec3 resultingAcceleration = acceleration + forceAccumulator * inverseMass;

    auto scaledAcceleration = resultingAcceleration * duration;

    velocity += scaledAcceleration;

    auto scaledVelocity = velocity * duration;

    position += scaledVelocity;

    velocity *= powf(damping, duration);

    clearAccumulator();
}

std::ostream &operator<<(std::ostream &stream, glm::vec3 &vector) {
    stream << "x: " << vector.x << ", y: " << vector.y << " z: " << vector.z << "\n";
    return stream;
}

/////////// Position functions
void Particle::setPosition(const glm::vec3 &passedPosition) {
    position = passedPosition;
}

void Particle::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

const glm::vec3 &Particle::getPosition() const {
    return position;
}

// void Particle::printPosition() const {
//     std:: cout << "Position: " << position;
// }
///////////

/////////// Velocity functions
void Particle::setVelocity(const glm::vec3 &passedVelocity) {
    velocity = passedVelocity;
}

void Particle::setVelocity(float x, float y, float z) {
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

const glm::vec3 &Particle::getVelocity() const {
    return velocity;
}

// void Particle::printVelocity() const {
//     std:: cout << "Velocity: " << velocity;
// }
///////////

/////////// Acceleration functions
void Particle::setAcceleration(const glm::vec3 &passedAcceleration) {
    acceleration = passedAcceleration;
}

void Particle::setAcceleration(float x, float y, float z) {
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

const glm::vec3 &Particle::getAcceleration() const {
    return acceleration;
}

// void Particle::printAcceleration() const {
//     std:: cout << "Acceleration: " << acceleration;
// }
///////////

void Particle::setMass(float passedMass) {
    if (passedMass <= 0.0f) {
        mass = 0.0f;
        inverseMass = 0.0f;
    }
    mass = passedMass;
    inverseMass = 1/mass;
}

const float &Particle::getInverseMass() const {
    return inverseMass;
}

const float &Particle::getMass() const {
    return mass;
}

void Particle::setDamping(float passedDamping) {
    damping = passedDamping;
}

const float &Particle::getDamping() const {
    return damping;
}

void Particle::setKinetic(float passedKinetic) {
    kinetic = passedKinetic;
}

const float &Particle::getKinetic() const {
    return kinetic;
}

void Particle::setRadius(float passedRadius) {
    radius = passedRadius;
}

const float &Particle::getRadius() const {
    return radius;
}

void Particle::addForce(const glm::vec3 &passedForce) {
    forceAccumulator += passedForce;
}

void Particle::addForce(float x, float y, float z) {
    forceAccumulator.x += x;
    forceAccumulator.y += y;
    forceAccumulator.z += z;
}

void Particle::clearAccumulator() {
    forceAccumulator = {};
}

void Particle::clearAllValues() {
    position = {};
    velocity = {};
    acceleration = {};
    forceAccumulator = {};

    inverseMass = 0.0f;
    mass = 0.0f;
    damping = 1.0f;
    kinetic = 0.0f;
    radius = 0.0f;
}

void Particle::update(float deltaTime) {
    integrate(deltaTime);
}

void Particle::setTrail(const glm::vec3 &passedPosition, const glm::vec3 &passedColor) {
    trail.position = passedPosition;
    trail.color = passedColor;
}

void Particle::setTrailColor(const float &particleSpeed) {
    //slower speeds
    if (particleSpeed >= 0.0f && particleSpeed <= 10.0f) {
        setTrail(position, {0.0f, 0.2f, 1.0f});
    } else if (particleSpeed >= 10.0f && particleSpeed <= 16.0f) {
        setTrail(position, {0.0, 0.8, 1.0});
    } else if (particleSpeed >= 16.0f && particleSpeed <= 22.0f) {
        setTrail(position, {1.0, 0.55, 0.0});
    } else if (particleSpeed >= 22.0f && particleSpeed <= 30.0f) {
        setTrail(position, {1.0, 0.22, 0.0});
    } else {
        setTrail(position, {1.0, 0.0, 0.0});
    }
    //Faster speeds
}

void clearTrail() {

}

const Particle::ParticleTrail &Particle::getTrail() {
    return trail;
}

void Particle::recordTrail(std::array<ParticleTrail, 1000> &passedPositions) {
    if (passedPositions.size() >= 1000) { // if size is greater than 100 position
        passedPositions.erase(passedPositions.begin()); // delete stale position
        passedPositions.emplace_back(trail); // place in newest positon
        return;
    }
    passedPositions.emplace_back(trail); // keep placing until 100
}