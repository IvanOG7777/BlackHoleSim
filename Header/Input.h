//
// Created by elder on 6/21/2026.
//

#ifndef PARTICLESIM_INPUT_H
#define PARTICLESIM_INPUT_H

#include <vector>
#include "GLFW/glfw3.h"

#include "Particle.h"
#include "Physics.h"

struct SceneState {
    Particle *particle;
    Particle *blackhole;
    std::vector<Vector3> *trailPositions;
    std:: vector<Particle> *particles;
};

void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif //PARTICLESIM_INPUT_H