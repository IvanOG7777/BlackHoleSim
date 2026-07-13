//
// Created by elder on 6/21/2026.
//

#include "../Header/Input.h"


// Call back function used to set different particles states, will see how this acts for 3d version
void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *state = static_cast<SceneState *>(glfwGetWindowUserPointer(window));


    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.0f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 1.0f);
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.0f);


        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 0.0f);
        }
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.2f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 0.2f);
        }
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.5f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 0.5f);
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 0.7f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 0.7f);
        }
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.2f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 1.2f);
        }
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        state->particle->clearTrail();
        setParticleOrbit(*state->particle, *state->blackhole, 1.5f);

        for (auto &particle: *state->particles) {
            particle.clearTrail();
            setParticleOrbit(particle, *state->blackhole, 1.5f);
        }
    }
}

void cursorPositionCallback(GLFWwindow *window, double positionX, double positionY) {
    auto *state = static_cast<SceneState *>(glfwGetWindowUserPointer(window));
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
