//
// Created by Ivan on 6/23/2026.
//
#ifndef PARTICLESIM_BLACKHOLE_H
#define PARTICLESIM_BLACKHOLE_H



#include "SphereParticle.h"

class Blackhole : public SphereParticle {
private:
    float MU;
    float captureRadius;
    float photonSphere;
    float eventHorizon;

public:
    Blackhole();

    void setMU(const float &passedMU);
    float getMU();

    void setCaptureRadius();
    float getCaptureRadius();

    void setPhotonSphere();
    float getPhotonSphere();

    void setEventHorizon(const float &passedEventHorizon);
    float getEventHorizon();
};

#endif // PARTICLESIM_BLACKHOLE_H