/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef MATH_H_
#define MATH_H_

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
using glm::quat;

#include "Hashes.h"

#endif
