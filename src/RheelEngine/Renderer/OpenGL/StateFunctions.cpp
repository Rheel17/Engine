/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateFunctions.h"

namespace rheel::GL {

StateFunctions::StateFunctions() :
		_parent(nullptr) {}

StateFunctions::StateFunctions(GL::StateFunctions *parent) :
		_parent(parent) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

void StateFunctions::SetClearColor(float red, float green, float blue, float alpha) {
	auto color = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (_GetClearColor() == color) {
		return;
	}

	// perform the state change
	glClearColor(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (color == _default_clear_color) : (color == _parent->_GetClearColor())) {
		_clear_color.reset();
	} else {
		_clear_color = color;
	}
}

void StateFunctions::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	SetBlendFunction(sfactor, dfactor, sfactor, dfactor);
}

void StateFunctions::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	auto function = std::make_tuple(srcRGB, dstRGB, srcAlpha, dstAlpha);

	// check if a state change is necessary
	if (_GetBlendFunction() == function) {
		return;
	}

	// perform the state change
	glBlendFuncSeparate(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));

	// store the state change
	if (_parent == nullptr ? (function == _default_blending_factors) : (function == _parent->_GetBlendFunction())) {
		_blending_factors.reset();
	} else {
		_blending_factors = function;
	}
}

void StateFunctions::SetDepthFunction(CompareFunction func) {
	// check if a state change is necessary
	if (_GetDepthFunction() == func) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(func));

	// store the state change
	if (_parent == nullptr ? (func == _default_depth_function) : (func == _parent->_GetDepthFunction())) {
		_depth_function.reset();
	} else {
		_depth_function = func;
	}
}

void StateFunctions::SetCullFace(CullFace mode) {
	// check if a state change is necessary
	if (_GetCullFace() == mode) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(mode));

	// store the state change
	if (_parent == nullptr ? (mode == _default_cull_face) : (mode == _parent->_GetCullFace())) {
		_cull_face.reset();
	} else {
		_cull_face = mode;
	}
}

void StateFunctions::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	auto mask = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (_GetColorMask() == mask) {
		return;
	}

	// perform the state change
	glColorMask(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (mask == _default_color_mask) : (mask == _parent->_GetColorMask())) {
		_color_mask.reset();
	} else {
		_color_mask = mask;
	}
}

void StateFunctions::SetDepthMask(bool mask) {
	// check if a state change is necessary
	if (_GetDepthMask() == mask) {
		return;
	}

	// perform the state change
	glDepthMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_depth_mask) : (mask == _parent->_GetDepthMask())) {
		_depth_mask.reset();
	} else {
		_depth_mask = mask;
	}
}

void StateFunctions::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	auto function = std::make_tuple(func, reference, mask);

	// check if a state change is necessary
	if (_GetStencilFunc() == function) {
		return;
	}

	// perform the state change
	glStencilFunc(GLenum(func), reference, mask);

	// store the state change
	if (_parent == nullptr ? (function == _default_stencil_func) : (function == _parent->_GetStencilFunc())) {
		_stencil_func.reset();
	} else {
		_stencil_func = function;
	}
}

void StateFunctions::SetStencilMask(uint8_t mask) {
	// check if a state change is necessary
	if (_GetStencilMask() == mask) {
		return;
	}

	// perform the state change
	glStencilMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_stencil_mask) : (mask == _parent->_GetStencilMask())) {
		_stencil_mask.reset();
	} else {
		_stencil_mask = mask;
	}
}

void StateFunctions::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	auto op = std::make_tuple(sfail, dpfail, dppass);

	// check if a state change is necessary
	if (_GetStencilOp() == op) {
		return;
	}

	// perform the state change
	glStencilOp(GLenum(sfail), GLenum(dpfail), GLenum(dppass));

	// store the state change
	if (_parent == nullptr ? (op == _default_stencil_op) : (op == _parent->_GetStencilOp())) {
		_stencil_op.reset();
	} else {
		_stencil_op = op;
	}
}

#pragma clang diagnostic pop

void StateFunctions::ResetChanges() {
	if (_clear_color.has_value()) {
		const auto& [red, green, blue, alpha] = *_clear_color;
		glClearColor(red, green, blue, alpha);
	}

	if (_blending_factors.has_value()) {
		const auto& [srcRGB, dstRGB, srcAlpha, dstAlpha] = *_blending_factors;
		glBlendFuncSeparate(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));
	}

	if (_depth_function.has_value()) {
		const auto& func = *_depth_function;
		glDepthFunc(GLenum(func));
	}

	if (_cull_face.has_value()) {
		const auto& cullFace = *_cull_face;
		glCullFace(GLenum(cullFace));
	}

	if (_color_mask.has_value()) {
		const auto& [red, green, blue, alpha] = *_color_mask;
		glColorMask(red, green, blue, alpha);
	}

	if (_depth_mask.has_value()) {
		const auto& depthMask = *_depth_mask;
		glDepthMask(depthMask);
	}

	if (_stencil_func.has_value()) {
		const auto& [func, ref, mask] = *_stencil_func;
		glStencilFunc(GLenum(func), ref, mask);
	}

	if (_stencil_mask.has_value()) {
		const auto& mask = *_stencil_mask;
		glStencilMask(mask);
	}

	if (_stencil_op.has_value()) {
		const auto& [sfail, dpfail, dppass] = *_stencil_op;
		glStencilOp(GLenum(sfail), GLenum(dpfail), GLenum(dppass));
	}

	_clear_color.reset();
	_blending_factors.reset();
	_depth_function.reset();
	_cull_face.reset();
	_color_mask.reset();
	_depth_mask.reset();
	_stencil_func.reset();
	_stencil_mask.reset();
	_stencil_op.reset();
}

std::tuple<float, float, float, float> StateFunctions::_GetClearColor() const {
	// check the current instance
	if (_clear_color.has_value()) {
		return *_clear_color;
	}

	// check parent
	if (_parent != nullptr) {
		return _parent->_GetClearColor();
	}

	// default
	return _default_clear_color;
}

std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor> StateFunctions::_GetBlendFunction() const {
	// check the current instance
	if (_blending_factors.has_value()) {
		return *_blending_factors;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetBlendFunction();
	}

	// default
	return _default_blending_factors;
}

CompareFunction StateFunctions::_GetDepthFunction() const {
	// check the current instance
	if (_depth_function.has_value()) {
		return *_depth_function;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetDepthFunction();
	}

	// default
	return _default_depth_function;
}

CullFace StateFunctions::_GetCullFace() const {
	// check the current instance
	if (_cull_face.has_value()) {
		return *_cull_face;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetCullFace();
	}

	// default
	return _default_cull_face;
}

std::tuple<bool, bool, bool, bool> StateFunctions::_GetColorMask() const {
	// check the current instance
	if (_color_mask.has_value()) {
		return *_color_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetColorMask();
	}

	// default
	return _default_color_mask;
}

bool StateFunctions::_GetDepthMask() const {
	// check the current instance
	if (_depth_mask.has_value()) {
		return *_depth_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetDepthMask();
	}

	// default
	return _default_depth_mask;
}

std::tuple<CompareFunction, uint8_t, uint8_t> StateFunctions::_GetStencilFunc() const {
	// check the current instance
	if (_stencil_func.has_value()) {
		return *_stencil_func;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetStencilFunc();
	}

	// default
	return _default_stencil_func;
}

uint8_t StateFunctions::_GetStencilMask() const {
	// check the current instance
	if (_stencil_mask.has_value()) {
		return *_stencil_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetStencilMask();
	}

	// default
	return _default_stencil_mask;
}

std::tuple<StencilFunction, StencilFunction, StencilFunction> StateFunctions::_GetStencilOp() const {
	// check the current instance
	if (_stencil_op.has_value()) {
		return *_stencil_op;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetStencilOp();
	}

	// default
	return _default_stencil_op;
}

}
