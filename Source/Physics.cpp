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

float orbitalSpeed(float mu, const float &radius) {
    float velocity = std::sqrtf(mu / radius);

    return velocity;
}


Vector3 orbitalTangent(Vector3 &BHPosition, Particle &particlePosition) {
    
}