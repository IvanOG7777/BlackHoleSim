//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_RENDERER_H
#define PARTICLESIM_RENDERER_H

#include <vector>
#include "Particle.h"
#include "../Header/Constants.h"
#include "glad/glad.h"

std::vector<Vector3> makeUnitCircle(const float &radius);

std::vector<std::vector<Vector3> > makeGrid(int slices);

void recordTrail(std::vector<Particle::ParticleTrail> &positions, const Particle::ParticleTrail &particlePosition);

void setVAO(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<Vector3> &vector3s);

void setTrailVao(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<Particle::ParticleTrail> &trails);

const char *makeVertexShader(const std::string &shaderType);

const char *makeFragmentShader(const std::string &shaderType);

GLuint createShader(const char *shader, GLenum shaderType);

#endif //PARTICLESIM_RENDERER_H