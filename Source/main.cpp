#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Header/Particle.h"

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

    Particle particle;
    particle.printPosition();
    particle.printVelocity();
    particle.printAcceleration();

    particle.setPosition(1,2,3);
    particle.setVelocity(4,5,6);
    particle.setAcceleration(7,8,9);
    particle.setMass(5);
    particle.setRadius(10);
    particle.setKinetic(5);

    std:: cout << std:: endl;
    particle.printPosition();
    particle.printVelocity();
    particle.printAcceleration();
    std:: cout << std:: endl;

    particle.addForce(10,0,0);

    for (int i = 0; i < 100; i++) {
        particle.update(0.016f);
        particle.printPosition();
        particle.printVelocity();
        particle.printAcceleration();
        std:: cout << std:: endl;
    }

    return 0;
}