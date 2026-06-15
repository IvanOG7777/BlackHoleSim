#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Header/Particle.h"

float COR = .65f;
float yVelThreshold = 0.2f;
// float ECC =

int main() {

    // glfwInit();
    //
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //
    // if (!glfwInit()) {
    //     std:: cerr << "GLFW INIT ERROR \n";
    //     return -1;
    // }
    //
    // GLFWwindow *window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    // if (window == nullptr) {
    //     std:: cerr << "WINDOW ERROR, COULDN'T CREATE, WINDOW IS NULL\n";
    //     glfwTerminate();
    //     return -1;
    // }
    //
    // glfwMakeContextCurrent(window);
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //     std::cerr << "GLAD INIT ERROR\n";
    //     return -1;
    // }
    //
    // while (!glfwWindowShouldClose(window)) {
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glfwPollEvents();
    //     glfwSwapBuffers(window);
    // }
    //
    // glfwDestroyWindow(window);
    // glfwTerminate();

    bool hitFloor = false;
    bool isStill = false;
    Particle particle;
    particle.printPosition();
    particle.printVelocity();
    particle.printAcceleration();

    particle.setPosition(0,10,0);
    particle.setVelocity(1,0,0);
    particle.setAcceleration(0,-9.8,0);
    particle.setDamping(1.0f);
    particle.setMass(5);
    particle.setRadius(10);
    particle.setKinetic(5);

    int loopCounter = 0;
    while (isStill == false) {
        particle.update(0.016f);
        if (particle.getPosition().y <= 0.0f) {
            Vector3 vel = particle.getVelocity();
            Vector3 pos = particle.getPosition();

            pos.y = 0;
            vel.y = -vel.y * COR;

            particle.setVelocity(vel);
            particle.setPosition(pos);
        }

        if (particle.getPosition().y == 0 && std::abs(particle.getVelocity().y) <= yVelThreshold) {
            particle.setAcceleration({});
            particle.setVelocity({});
            isStill = true;
        }
        particle.printPosition();
        particle.printVelocity();
        particle.printAcceleration();
        loopCounter++;
        std:: cout << std:: endl;
    }

    std:: cout << "Loops done: " << loopCounter << std:: endl;

    return 0;
}
