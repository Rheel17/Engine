/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateFunctions.h"

namespace rheel::GL {

StateFunctions::StateFunctions() :
		_parent(nullptr) {}

StateFunctions::StateFunctions(GL::StateFunctions *parent) :
		_parent(parent) {}

void StateFunctions::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	SetBlendFunction(sfactor, dfactor, sfactor, dfactor);
}

void StateFunctions::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	auto function = std::make_tuple(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));

	// check if a state change is necessary
	if (_GetBlendFunction() == function) {
		return;
	}

	// perform the state change
	glBlendFuncSeparate(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));

	// store the state change
	if (_parent->_GetBlendFunction() == function) {
		// this was a reversal of the current change
		_blending_factors.reset();
	} else {
		// this is a state change
		_blending_factors = function;
	}
}

void StateFunctions::ResetChanges() {
	if (_blending_factors.has_value()) {
		const auto& [srcRGB, dstRGB, srcAlpha, dstAlpha] = *_blending_factors;
		glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	}

	_blending_factors.reset();
}

std::tuple<GLenum, GLenum, GLenum, GLenum> StateFunctions::_GetBlendFunction() const {
	// check the current instance
	if (_blending_factors.has_value()) {
		return *_blending_factors;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetBlendFunction();
	}

	// default
	return { GL_ONE, GL_ZERO, GL_ONE, GL_ZERO };
}

}