//
// Created by elder on 6/26/2026.
//

#ifndef PARTICLESIM_GLUTILS_H
#define PARTICLESIM_GLUTILS_H

#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Particle.h"
#include "glm/glm/detail/type_vec.hpp"

void setVAO(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<glm::vec3> &vector3s);

void setTrailVao(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::array<Particle::ParticleTrail, 1000> &trails);

const char *makeVertexShader(const std::string &shaderType);

const char *makeFragmentShader(const std::string &shaderType);

GLuint createShader(const char *shader, GLenum shaderType);

GLFWwindow *createWindow(int width, int height, const char *windowName);


#endif //PARTICLESIM_GLUTILS_H