/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MATH_H
#define RHEELENGINE_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#include "glm_debug.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::dvec1;
using glm::dvec2;
using glm::dvec3;
using glm::dvec4;
using glm::ivec1;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::uvec1;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::mat2x3;
using glm::mat2x4;
using glm::mat3x2;
using glm::mat3x4;
using glm::mat4x2;
using glm::mat4x3;
using glm::dmat2;
using glm::dmat3;
using glm::dmat4;
using glm::dmat2x3;
using glm::dmat2x4;
using glm::dmat3x2;
using glm::dmat3x4;
using glm::dmat4x2;
using glm::dmat4x3;
using glm::quat;

#include "Hashes.h"

#endif
