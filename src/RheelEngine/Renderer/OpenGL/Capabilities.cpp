/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Capabilities.h"

namespace rheel::gl {

std::unordered_map<GLenum, int> Capabilities::_integer_capabilities;
std::unordered_map<GLenum, float> Capabilities::_float_capabilities;

int Capabilities::GetMaxVertexAttribs() {
	return _get_integer(GL_MAX_VERTEX_ATTRIBS);
}

float Capabilities::GetMaxTextureMaxAnisotropy() {
	return _get_float(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
}

int Capabilities::_get_integer(GLenum cap) {
	auto iter = _integer_capabilities.find(cap);

	if (iter == _integer_capabilities.end()) {
		GLint value;
		glGetIntegerv(cap, &value);

		iter = _integer_capabilities.emplace(cap, value).first;
	}

	return iter->second;
}

float Capabilities::_get_float(GLenum cap) {
	auto iter = _float_capabilities.find(cap);

	if (iter == _float_capabilities.end()) {
		GLfloat value;
		glGetFloatv(cap, &value);

		iter = _float_capabilities.emplace(cap, value).first;
	}

	return iter->second;
}

}