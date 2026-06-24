//
// Created by elder on 6/19/2026.
//

#ifndef PARTICLESIM_RENDERER_H
#define PARTICLESIM_RENDERER_H

#include <vector>
#include "Core.h"
#include "../Header/Constants.h"
#include "glad/glad.h"

std:: vector<Vector3> makeUnitCircle(const float &radius);

std:: vector<std::vector<Vector3>> makeGrid(int slices);

void recordTrail(std:: vector<Vector3> &positions, const Vector3 &particlePosition);

void setVAO(GLuint &VAO, GLuint &VBO, GLenum drawHint, std::vector<Vector3> &vector3s);

#endif //PARTICLESIM_RENDERER_H