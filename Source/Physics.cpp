//
// Created by elder on 6/19/2026.
//

#include <random>
#include "../Header/Physics.h"

void keepInFrame(Particle &particle) {
    float radius = particle.getRadius();
    Vector3 pos = particle.getPosition();
    Vector3 vel = particle.getVelocity();

    float minX = radius;
    float maxX = W - radius;
    float minY = radius;
    float maxY = H - radius;

    if (pos.x < minX) {
        pos.x = minX;
        vel.x = -vel.x * COR;
    }

    if (pos.x > maxX) {
        pos.x = maxX;
        vel.x = -vel.x * COR;
    }

    if (pos.y < minX) {
        pos.y = minY;
        vel.y = -vel.y * COR;
    }

    if (pos.y > maxY) {
        pos.y = maxY;
        vel.y = -vel.y * COR;
    }

    particle.setPosition(pos);
    particle.setVelocity(vel);
}

Vector3 gravitationalAcceleration(const Vector3 &BHPosition, const Vector3 &particlePosition, float mu) {

    Vector3 direction = BHPosition - particlePosition; // gets direction vector
    float distanceSquared = direction.squareMagnitude(); // gets distance

    if (distanceSquared <= 0.0f) return {};

    float distance = std:: sqrtf(distanceSquared);
    float distanceCubed = distanceSquared * distance;

    Vector3 acceleration = (direction * mu) / distanceCubed; // calculates acceleration

    return acceleration;
}

bool hasBeenCaptured(const Vector3 &BHPosition, const Vector3 &particlePosition, float captureRadius) {
    Vector3 direction = BHPosition - particlePosition;

    float distance = direction.magnitude();

    return distance <= captureRadius;
}

float orbitalSpeed(const float &mu, const float &radius) {
    float speed = std::sqrtf(mu / radius);

    return speed;
}


Vector3 orbitalTangent(const Vector3 &BHPosition, const Vector3 &particlePosition) {
    Vector3 tangent = BHPosition - particlePosition; // get direction;

    float x = tangent.x;
    float y = tangent.y;

    // Swap values and make perpendicular;
    tangent.x = -y;
    tangent.y = x;

    // normalize new vector
    tangent.normalize();

    // returns the direction particle should be traveling in the y direction to combat gravitational pull
    // return normalized vector of y = 1 or y = -1
    return tangent;
}

Vector3 circularVelocity(const Vector3& BHPosition, const Vector3& particlePosition, const float &mu, const float &radius) {
    float speed = orbitalSpeed(mu, radius);
    std:: cout << "Speed: " << speed << "\n";
    Vector3 tangent = orbitalTangent(BHPosition, particlePosition);

    Vector3 velocity = tangent * speed;

    return velocity;
}

float orbitalRadius(const Vector3& BHPosition, const Vector3& particlePosition) {
    Vector3 displacement = BHPosition - particlePosition;

    float radius = displacement.magnitude();

    return radius;
}

void setParticleOrbit(Particle &particle, Particle &BH, const float &velocityMultiplayer) {
    float minPosX = 100.0f, maxPosX = 1820.0f;
    float minPosY = 100.0f, maxPosY = 920.0f;
    float minRadius = 5.0f, maxRadius = 15.0f;

    std::random_device rd;
    std::mt19937 genPosX(rd());
    std::mt19937 genPosY(rd());
    std::mt19937 genRadius(rd());

    std::uniform_int_distribution<> posXDis(minPosX, maxPosX);
    std::uniform_int_distribution<> posYDis(minPosY, maxPosY);
    std::uniform_int_distribution<> radiusDis(minRadius, maxRadius);
    float damp = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    float posX = posXDis(genPosX);
    float posY = posYDis(genPosY);
    float radius = radiusDis(genRadius);

    particle.setPosition(posX, posY, 0);
    particle.setDamping(.999f);
    particle.setMass(5);
    particle.setRadius(radius);

    float initOrbitalRadius = orbitalRadius(BH.getPosition(), particle.getPosition());
    Vector3 velocity = circularVelocity(BH.getPosition(), particle.getPosition(), MU, initOrbitalRadius);

    velocity *= velocityMultiplayer;

    particle.setVelocity(velocity);
}

float speed(const Vector3 &velocity) {
    return velocity.magnitude();
}

float orbitalEnergy(const Vector3 &BHPosition, const Vector3 &particlePosition, const Vector3 &velocity, float mu) {
    Vector3 radialDisplacement = BHPosition - particlePosition;

    float radius = radialDisplacement.magnitude();

    auto velSquared = velocity * velocity;

    float energy = (0.5 * velSquared) - (mu / radius);

    return energy;
}

float angularMomentum(const Vector3 &BHPosition, const Vector3 &particlePosition, const Vector3 &particleVelocity) {
    Vector3 radialDisplacement = BHPosition - particlePosition;

    auto momentum = (radialDisplacement.x * particleVelocity.y) - (radialDisplacement.y * particleVelocity.x);

    return momentum;
}

std::string orbitType(const float &particleEnergy) {
    if (particleEnergy < 0) return "Bounded";

    if (particleEnergy == 0) return "Escape Boundry";

    if (particleEnergy > 0) return "Escape Trajectory";

    return "Bad input";
}