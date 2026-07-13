//
// Created by elder on 6/21/2026.
//

#include "../Header/Input.h"


// Call back function used to set different particles states, will see how this acts for 3d version
void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *state = static_cast<SceneState *>(glfwGetWindowUserPointer(window));
    auto *particleTrails = state->positions;
    auto *particles = state->particles;


    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.0f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 1.0f);
            index++;
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.0f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 0.0f);
            index++;
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.2f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 0.2f);
            index++;
        }
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.5f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 0.5f);
            index++;
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.7f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 0.7f);
            index++;
        }
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.2f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 1.2f);
            index++;
        }
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.5f);

        int index = 0;
        for (auto &particle : *state->particles) {
            auto &currentPosition = (*state->positions)[index];
            currentPosition = {};
            setParticleOrbit(particle, *state->blackhole, 1.5f);
            index++;
        }
    }
}

void cursorPositionCallback(GLFWwindow *window, double positionX, double positionY) {
    auto *state = static_cast<SceneState*>(glfwGetWindowUserPointer(window));
    auto *camera = state->camera;

    //
    if (camera->firstMove == true) {
        camera->currentX = static_cast<float>(positionX);
        camera->currentY = static_cast<float>(positionY);
        camera->firstMove = false;
        return;
    }

    float deltaX = camera->currentX - static_cast<float>(positionX);
    float deltaY = camera->currentY - static_cast<float>(positionY);

    float pitch = camera->getPitch();
    float yaw = camera->getYaw();

    yaw -= deltaX * 0.001f;
    pitch += deltaY * 0.001f;

    float maxPitch = glm::radians(89.0f);

    if (pitch < -maxPitch) {
        pitch = -maxPitch;
    }

    if (pitch > maxPitch) {
        pitch = maxPitch;
    }

    camera->setYaw(yaw);
    camera->setPitch(pitch);

    camera->currentX = static_cast<float>(positionX);
    camera->currentY = static_cast<float>(positionY);
}
