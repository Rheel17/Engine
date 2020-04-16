/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_GLM_DEBUG_H
#define RHEELENGINE_GLM_DEBUG_H
#include "../_common.h"

#include <iostream>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

RE_API std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec);
RE_API std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec);
RE_API std::ostream& operator<<(std::ostream& stream, const glm::vec4& vec);
RE_API std::ostream& operator<<(std::ostream& stream, const glm::quat& rot);

RE_API std::ostream& operator<<(std::ostream& stream, const glm::mat2& mat);
RE_API std::ostream& operator<<(std::ostream& stream, const glm::mat3& mat);
RE_API std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat);

#endif
