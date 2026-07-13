//
// Created by elder on 6/21/2026.
//

#ifndef PARTICLESIM_INPUT_H
#define PARTICLESIM_INPUT_H

#include <vector>
#include "GLFW/glfw3.h"

#include "Particle.h"
#include "Physics.h"
#include "Camera.h"

struct SceneState {
    Camera *camera;
    SphereParticle *particle;
    SphereParticle *blackhole;
    std::array<Particle::ParticleTrail, 1000> *trailPositions;
    std::vector<std::array<Particle::ParticleTrail,1000>> *positions;
    std:: vector<SphereParticle> *particles;
};

void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);

void cursorPositionCallback(GLFWwindow *window, double positionX, double positionY);

#endif //PARTICLESIM_INPUT_H