/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "glm_debug.h"

#define FP "% 9.4f"

template<typename... Args>
std::string string_format(const std::string& format, Args... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
	std::unique_ptr<char[]> buf(new char[ size ]);
	snprintf(buf.get(), size, format.c_str(), args...);
	return std::string(buf.get(), buf.get() + size - 1);
}

std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec) {
	return stream << string_format("(" FP ", " FP ")", vec.x, vec.y);
}

std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec) {
	return stream << string_format("(" FP ", " FP ", " FP ")", vec.x, vec.y, vec.z);
}

std::ostream& operator<<(std::ostream& stream, const glm::vec4& vec) {
	return stream << string_format("(" FP ", " FP ", " FP ", " FP ")", vec.x, vec.y, vec.z, vec.w);
}

std::ostream& operator<<(std::ostream& stream, const glm::quat& rot) {
	return stream << string_format("(" FP "i , " FP " j, " FP " k, " FP ")", rot.x, rot.y, rot.z, rot.w);
}

std::ostream& operator<<(std::ostream& stream, const glm::mat2& mat) {
	return stream
			<< string_format("(" FP ", " FP " \n", mat[0][0], mat[1][0])
			<< string_format(" " FP ", " FP ")\n", mat[0][1], mat[1][1]);
}

std::ostream& operator<<(std::ostream& stream, const glm::mat3& mat) {
	return stream
			<< string_format("(" FP ", " FP ", " FP " \n", mat[0][0], mat[1][0], mat[2][0])
			<< string_format(" " FP ", " FP ", " FP " \n", mat[0][1], mat[1][1], mat[2][1])
			<< string_format(" " FP ", " FP ", " FP ")\n", mat[0][2], mat[1][2], mat[2][2]);
}

std::ostream& operator<<(std::ostream& stream, const glm::mat4& mat) {
	return stream
			<< string_format("(" FP ", " FP ", " FP ", " FP " \n", mat[0][0], mat[1][0], mat[2][0], mat[3][0])
			<< string_format(" " FP ", " FP ", " FP ", " FP " \n", mat[0][1], mat[1][1], mat[2][1], mat[3][1])
			<< string_format(" " FP ", " FP ", " FP ", " FP " \n", mat[0][2], mat[1][2], mat[2][2], mat[3][2])
			<< string_format(" " FP ", " FP ", " FP ", " FP ")\n", mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
}
