//
// Created by Ivan on 6/23/2026.
//

#include "../Header/Blackhole.h"
#include "../Header/Constants.h"

Blackhole::Blackhole() {
    MU = 0.0f;
    captureRadius = 0.0f;
    photonSphere = 0.0f;
    eventHorizon = 0.0f;
}

void Blackhole::setMU(const float &passedMU) {
    MU = passedMU;
}

float Blackhole::getMU() {
    return MU;
}

void Blackhole::setCaptureRadius() {
    if (MU == 0.0f) {
        std:: cerr << "MU IS 0.0f returning" << std:: endl;
        exit(EXIT_FAILURE);
    }
    float radius = (2 * MU) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);

    captureRadius = radius;
}

float Blackhole::getCaptureRadius() {
    return captureRadius;
}

void Blackhole::setPhotonSphere() {
    if (MU == 0.0f) {
        std:: cerr << "MU IS 0.0f returning" << std:: endl;
        exit(EXIT_FAILURE);
    }
    float radius = (3 * MU) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
    photonSphere = radius;
}

float Blackhole::getPhotonSphere() {
    return photonSphere;
}

void Blackhole::setEventHorizon(const float& passedEventHorizon) {
    eventHorizon = passedEventHorizon;
}

float Blackhole::getEventHorizon() {
    return eventHorizon;
}

