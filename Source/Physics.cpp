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
