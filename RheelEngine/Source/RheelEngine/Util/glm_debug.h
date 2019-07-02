#ifndef GLM_DEBUG_H_
#define GLM_DEBUG_H_

#include <iostream>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& stream, const glm::vec4& vec);
std::ostream& operator<<(std::ostream& stream, const glm::quat& rot);

std::ostream& operator<<(std::ostream& stream, const glm::mat2& mat);
std::ostream& operator<<(std::ostream& stream, const glm::mat3& mat);
std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat);

#endif
