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
    radius = 0.0f;
    head = 0;
    tail = 0;
    count = 0;
}

Particle::ParticleTrail::ParticleTrail() {
    position = {0.0f, 0.0f, 0.0f};
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
        return;
    }
    mass = passedMass;
    inverseMass = 1 / mass;
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
    float BLUE = 0.0f;
    float CYAN = 10.0f;
    float ORANGE = 16.0f;
    float RED_ORANGE = 22.0f;
    float RED = 30.0f;
    float t = 0.0f;

    if (particleSpeed >= BLUE && particleSpeed <= CYAN) {
        t = (particleSpeed - BLUE) / (CYAN - BLUE);
        auto resultingColor = glm::vec3{0.0f, 0.2f, 1.0f} + (t * (glm::vec3{0.0f, 0.8f, 1.0f} - glm::vec3{0.0f, 0.2f, 1.0f}));
        setTrail(position, resultingColor);
    } else if (particleSpeed >= CYAN && particleSpeed <= ORANGE) {
        t = (particleSpeed - CYAN) / (ORANGE - CYAN);
        auto resultingColor = glm::vec3{0.0f, 0.8f, 1.0f} + (t * (glm::vec3{1.0, 0.55, 0.0} - glm::vec3{0.0f, 0.8f, 1.0f}));
        setTrail(position, resultingColor);
    } else if (particleSpeed >= ORANGE && particleSpeed <= RED_ORANGE) {
        t = (particleSpeed - ORANGE) / (RED_ORANGE - ORANGE);
        auto resultingColor = glm::vec3{1.0, 0.55, 0.0} + (t * (glm::vec3{1.0, 0.22, 0.0} - glm::vec3{1.0, 0.55, 0.0}));
        setTrail(position, resultingColor);
    } else {
        t = (particleSpeed - RED) / (RED - RED_ORANGE);
        auto resultingColor = glm::vec3{1.0, 0.22, 0.0} + (t * (glm::vec3{1.0, 0.0, 0.0} - glm::vec3{1.0, 0.22, 0.0}));
        setTrail(position, resultingColor);
    }
    //Faster speeds
}

void Particle::clearTrail() {
    trailPositions = {};
    head = 0;
    tail = 0;
    count = 0;
}

const Particle::ParticleTrail &Particle::getTrail() {
    return trail;
}

void Particle::recordTrail(const ParticleTrail &passedTrail) {
    overwrite(passedTrail);
}

std::array<Particle::ParticleTrail, 1000> &Particle::getTrailPositons() {
    return trailPositions;
}

bool Particle::isFull() {
    return count == trailPositions.size();
}

bool Particle::isEmpty() {
    return count == 0;
}

size_t Particle::overwrite(const ParticleTrail &passedTrail) {
    if (isFull()) {
        trailPositions[tail] = passedTrail;
        head = (head + 1) % trailPositions.size();
        tail = (tail + 1) % trailPositions.size();
    } else {
        trailPositions[tail] = passedTrail;
        tail = (tail + 1) % trailPositions.size();
        count++;
    }
    return 1;
}

size_t Particle::getHead() {
    return head;
}

size_t Particle::getTail() {
    return tail;
}

size_t Particle::getCount() {
    return count;
}