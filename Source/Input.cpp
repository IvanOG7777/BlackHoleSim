//
// Created by elder on 6/21/2026.
//

#include "../Header/Input.h"


void keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *state = static_cast<SceneState *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 1.0f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 0.0f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 0.2f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 0.5f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 0.7f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 1.2f);
        state->trailPositions->clear();
    }

    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        setParticle(*state->particle, *state->blackhole, 1.5f);
        state->trailPositions->clear();
    }
}