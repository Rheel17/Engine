/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateFunctions.h"

namespace rheel::gl {

StateFunctions::StateFunctions() :
		_parent(nullptr) {}

StateFunctions::StateFunctions(gl::StateFunctions* parent) :
		_parent(parent) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

void StateFunctions::SetClearColor(float red, float green, float blue, float alpha) {
	auto color = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (GetClearColor_() == color) {
		return;
	}

	// perform the state change
	glClearColor(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (color == _default_clear_color) : (color == _parent->GetClearColor_())) {
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
	if (GetBlendFunction_() == function) {
		return;
	}

	// perform the state change
	glBlendFuncSeparate(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));

	// store the state change
	if (_parent == nullptr ? (function == _default_blending_factors) : (function == _parent->GetBlendFunction_())) {
		_blending_factors.reset();
	} else {
		_blending_factors = function;
	}
}

void StateFunctions::SetDepthFunction(CompareFunction func) {
	// check if a state change is necessary
	if (GetDepthFunction_() == func) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(func));

	// store the state change
	if (_parent == nullptr ? (func == _default_depth_function) : (func == _parent->GetDepthFunction_())) {
		_depth_function.reset();
	} else {
		_depth_function = func;
	}
}

void StateFunctions::SetCullFace(CullFace mode) {
	// check if a state change is necessary
	if (GetCullFace_() == mode) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(mode));

	// store the state change
	if (_parent == nullptr ? (mode == _default_cull_face) : (mode == _parent->GetCullFace_())) {
		_cull_face.reset();
	} else {
		_cull_face = mode;
	}
}

void StateFunctions::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	auto mask = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (GetColorMask_() == mask) {
		return;
	}

	// perform the state change
	glColorMask(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (mask == _default_color_mask) : (mask == _parent->GetColorMask_())) {
		_color_mask.reset();
	} else {
		_color_mask = mask;
	}
}

void StateFunctions::SetDepthMask(bool mask) {
	// check if a state change is necessary
	if (GetDepthMask_() == mask) {
		return;
	}

	// perform the state change
	glDepthMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_depth_mask) : (mask == _parent->GetDepthMask_())) {
		_depth_mask.reset();
	} else {
		_depth_mask = mask;
	}
}

void StateFunctions::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	auto function = std::make_tuple(func, reference, mask);

	// check if a state change is necessary
	if (GetStencilFunc_() == function) {
		return;
	}

	// perform the state change
	glStencilFunc(GLenum(func), reference, mask);

	// store the state change
	if (_parent == nullptr ? (function == _default_stencil_func) : (function == _parent->GetStencilFunc_())) {
		_stencil_func.reset();
	} else {
		_stencil_func = function;
	}
}

void StateFunctions::SetStencilMask(uint8_t mask) {
	// check if a state change is necessary
	if (GetStencilMask_() == mask) {
		return;
	}

	// perform the state change
	glStencilMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_stencil_mask) : (mask == _parent->GetStencilMask_())) {
		_stencil_mask.reset();
	} else {
		_stencil_mask = mask;
	}
}

void StateFunctions::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	auto op = std::make_tuple(sfail, dpfail, dppass);

	// check if a state change is necessary
	if (GetStencilOp_() == op) {
		return;
	}

	// perform the state change
	glStencilOp(GLenum(sfail), GLenum(dpfail), GLenum(dppass));

	// store the state change
	if (_parent == nullptr ? (op == _default_stencil_op) : (op == _parent->GetStencilOp_())) {
		_stencil_op.reset();
	} else {
		_stencil_op = op;
	}
}

#pragma clang diagnostic pop

void StateFunctions::ResetChanges() {
	if (_clear_color.has_value()) {
		const auto& [red, green, blue, alpha] = _parent == nullptr ? _default_clear_color : _parent->GetClearColor_();
		glClearColor(red, green, blue, alpha);
	}

	if (_blending_factors.has_value()) {
		const auto& [srcRGB, dstRGB, srcAlpha, dstAlpha] = _parent == nullptr ? _default_blending_factors : _parent->GetBlendFunction_();
		glBlendFuncSeparate(GLenum(srcRGB), GLenum(dstRGB), GLenum(srcAlpha), GLenum(dstAlpha));
	}

	if (_depth_function.has_value()) {
		const auto& func = _parent == nullptr ? _default_depth_function : _parent->GetDepthFunction_();
		glDepthFunc(GLenum(func));
	}

	if (_cull_face.has_value()) {
		const auto& cullFace = _parent == nullptr ? _default_cull_face : _parent->GetCullFace_();
		glCullFace(GLenum(cullFace));
	}

	if (_color_mask.has_value()) {
		const auto& [red, green, blue, alpha] = _parent == nullptr ? _default_color_mask : _parent->GetColorMask_();
		glColorMask(red, green, blue, alpha);
	}

	if (_depth_mask.has_value()) {
		const auto& depthMask = _parent == nullptr ? _default_depth_mask : _parent->GetDepthMask_();
		glDepthMask(depthMask);
	}

	if (_stencil_func.has_value()) {
		const auto& [func, ref, mask] = _parent == nullptr ? _default_stencil_func : _parent->GetStencilFunc_();
		glStencilFunc(GLenum(func), ref, mask);
	}

	if (_stencil_mask.has_value()) {
		const auto& mask = _parent == nullptr ? _default_stencil_mask : _parent->GetStencilMask_();
		glStencilMask(mask);
	}

	if (_stencil_op.has_value()) {
		const auto& [sfail, dpfail, dppass] = _parent == nullptr ? _default_stencil_op : _parent->GetStencilOp_();
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

std::tuple<float, float, float, float> StateFunctions::GetClearColor_() const {
	// check the current instance
	if (_clear_color.has_value()) {
		return *_clear_color;
	}

	// check parent
	if (_parent != nullptr) {
		return _parent->GetClearColor_();
	}

	// default
	return _default_clear_color;
}

std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor> StateFunctions::GetBlendFunction_() const {
	// check the current instance
	if (_blending_factors.has_value()) {
		return *_blending_factors;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetBlendFunction_();
	}

	// default
	return _default_blending_factors;
}

CompareFunction StateFunctions::GetDepthFunction_() const {
	// check the current instance
	if (_depth_function.has_value()) {
		return *_depth_function;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetDepthFunction_();
	}

	// default
	return _default_depth_function;
}

CullFace StateFunctions::GetCullFace_() const {
	// check the current instance
	if (_cull_face.has_value()) {
		return *_cull_face;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetCullFace_();
	}

	// default
	return _default_cull_face;
}

std::tuple<bool, bool, bool, bool> StateFunctions::GetColorMask_() const {
	// check the current instance
	if (_color_mask.has_value()) {
		return *_color_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetColorMask_();
	}

	// default
	return _default_color_mask;
}

bool StateFunctions::GetDepthMask_() const {
	// check the current instance
	if (_depth_mask.has_value()) {
		return *_depth_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetDepthMask_();
	}

	// default
	return _default_depth_mask;
}

std::tuple<CompareFunction, uint8_t, uint8_t> StateFunctions::GetStencilFunc_() const {
	// check the current instance
	if (_stencil_func.has_value()) {
		return *_stencil_func;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetStencilFunc_();
	}

	// default
	return _default_stencil_func;
}

uint8_t StateFunctions::GetStencilMask_() const {
	// check the current instance
	if (_stencil_mask.has_value()) {
		return *_stencil_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetStencilMask_();
	}

	// default
	return _default_stencil_mask;
}

std::tuple<StencilFunction, StencilFunction, StencilFunction> StateFunctions::GetStencilOp_() const {
	// check the current instance
	if (_stencil_op.has_value()) {
		return *_stencil_op;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetStencilOp_();
	}

	// default
	return _default_stencil_op;
}

}
