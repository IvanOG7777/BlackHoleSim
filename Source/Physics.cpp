//
// Created by elder on 6/19/2026.
//

#include <random>
#include "../Header/Physics.h"

glm::vec3 gravitationalAcceleration(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, float mu) {

    glm::vec3 direction = BHPosition - particlePosition; // gets direction vector
    float distanceSquared = glm::length(direction) * glm::length(direction); // gets distance

    if (distanceSquared <= 0.0f) return {};

    float distance = std:: sqrtf(distanceSquared);
    float distanceCubed = distanceSquared * distance;

    glm::vec3 acceleration = (direction * mu) / distanceCubed; // calculates acceleration

    return acceleration;
}

bool hasBeenCaptured(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, float captureRadius) {
    glm::vec3 direction = BHPosition - particlePosition;

    float distance = glm::length(direction);

    return distance <= captureRadius;
}

float orbitalSpeed(const float &mu, const float &radius) {
    float speed = std::sqrtf(mu / radius);

    return speed;
}


glm::vec3 orbitalTangent(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition) {
    glm::vec3 tangent = BHPosition - particlePosition; // get direction;

    float x = tangent.x;
    float z = tangent.z;

    // Swap values and make perpendicular;
    tangent.x = -z;
    tangent.y = x;

    // normalize new vector
    tangent = glm::normalize(tangent);

    // returns the direction particle should be traveling in the y direction to combat gravitational pull
    // return normalized vector of y = 1 or y = -1
    return tangent;
}

glm::vec3 circularVelocity(const glm::vec3& BHPosition, const glm::vec3& particlePosition, const float &mu, const float &radius) {
    float speed = orbitalSpeed(mu, radius);
    glm::vec3 tangent = orbitalTangent(BHPosition, particlePosition);

    glm::vec3 velocity = tangent * speed;

    return velocity;
}

float orbitalRadius(const glm::vec3& BHPosition, const glm::vec3& particlePosition) {
    glm::vec3 displacement = BHPosition - particlePosition;

    float radius = glm::length(displacement);

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
    glm::vec3 velocity = circularVelocity(BH.getPosition(), particle.getPosition(), MU, initOrbitalRadius);

    velocity *= velocityMultiplayer;

    auto particleSpeed = speed(velocity);

    particle.setTrailColor(particleSpeed);

    particle.setVelocity(velocity);
}

float speed(const glm::vec3 &velocity) {
    return glm::length(velocity);
}

float orbitalEnergy(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, const glm::vec3 &velocity, float mu) {
    glm::vec3 radialDisplacement = BHPosition - particlePosition;

    float radius = glm::length(radialDisplacement);

    auto velSquared = glm::dot(velocity, velocity);

    float energy = (0.5f * velSquared) - (mu / radius);

    return energy;
}

float angularMomentum(const glm::vec3 &BHPosition, const glm::vec3 &particlePosition, const glm::vec3 &particleVelocity) {
    glm::vec3 radialDisplacement = BHPosition - particlePosition;

    auto momentum = (radialDisplacement.x * particleVelocity.y) - (radialDisplacement.y * particleVelocity.x);

    return momentum;
}

std::string orbitType(const float &particleEnergy) {
    if (particleEnergy < 0) return "Bounded";

    if (particleEnergy == 0) return "Escape Boundry";

    if (particleEnergy > 0) return "Escape Trajectory";

    return "Bad input";
}

void setDisk(Blackhole &blackhole, Particle &particle) {
    float innerRadius = blackhole.getPhotonSphere();
    float outerRadius = innerRadius + 200.0f;
    float angle1 = 0.0f;
    float angle2 = 2 * PI;

    float bhX = blackhole.getPosition().x;
    float bhZ = blackhole.getPosition().z;

    std::random_device rd;
    std::mt19937 genRadius(rd());
    std::mt19937 genAngle(rd());


    std::uniform_real_distribution<> radiusDis(innerRadius, outerRadius);
    std::uniform_real_distribution<> angleDis(angle1, angle2);

    auto randRadius = static_cast<float>(radiusDis(genRadius));
    auto randAngle = static_cast<float>(angleDis(genAngle));

    float theta = randAngle;
    float x = bhX + (randRadius * std::cosf(theta));
    float z = bhZ + (randRadius * std::sinf(theta));

    particle.setPosition(x, 0, z);
    particle.setMass(5);
    particle.setDamping(0.99);

    float radius = orbitalRadius(blackhole.getPosition(), {x,0,z});

    glm::vec3 velocity = circularVelocity(blackhole.getPosition(), particle.getPosition(), blackhole.getMU(), radius);

    auto particleSpeed = speed(velocity);

    particle.setTrailColor(particleSpeed);

    particle.setVelocity(velocity);
}
