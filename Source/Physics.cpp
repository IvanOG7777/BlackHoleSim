//
// Created by elder on 6/19/2026.
//

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
    std:: cout << speed << "\n";
    Vector3 tangent = orbitalTangent(BHPosition, particlePosition);

    Vector3 velocity = tangent * speed;

    return velocity;
}

float orbitalRadius(const Vector3& BHPosition, const Vector3& particlePosition) {
    Vector3 displacement = BHPosition - particlePosition;

    float radius = displacement.magnitude();

    return radius;
}

void setParticle(Particle &particle, Particle &BH, const float &velocityMultiplayer) {

    BH.setPosition(960, 540, 0);
    BH.setRadius(25);
    BH.setMass(100);

    particle.setPosition(1260, 540, 0);
    particle.setDamping(1.0f);
    particle.setMass(5);
    particle.setRadius(10);

    float initOrbitalRadius = orbitalRadius(BH.getPosition(), particle.getPosition());
    Vector3 velocity = circularVelocity(BH.getPosition(), particle.getPosition(), MU, initOrbitalRadius);

    velocity *= velocityMultiplayer;

    particle.setVelocity(velocity);
}