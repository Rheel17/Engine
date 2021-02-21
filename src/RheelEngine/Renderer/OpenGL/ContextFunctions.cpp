/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ContextFunctions.h"

#include "Context.h"

namespace rheel::gl {

ContextFunctions::ContextFunctions(Context& context) :
		_context(context),
		_parent(nullptr) {}

ContextFunctions::ContextFunctions(gl::ContextFunctions* parent) :
		_context(parent->_context),
		_parent(parent) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

void ContextFunctions::SetClearColor(float red, float green, float blue, float alpha) {
	auto color = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (_get_clear_color() == color) {
		return;
	}

	// perform the state change
	glClearColor(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (color == _default_clear_color) : (color == _parent->_get_clear_color())) {
		_clear_color.reset();
	} else {
		_clear_color = color;
	}
}

void ContextFunctions::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	SetBlendFunction(sfactor, dfactor, sfactor, dfactor);
}

void ContextFunctions::SetBlendFunction(BlendFactor src_rgb, BlendFactor dst_rgb, BlendFactor src_alpha, BlendFactor dst_alpha) {
	auto function = std::make_tuple(src_rgb, dst_rgb, src_alpha, dst_alpha);

	// check if a state change is necessary
	if (_get_blend_function() == function) {
		return;
	}

	// perform the state change
	glBlendFuncSeparate(GLenum(src_rgb), GLenum(dst_rgb), GLenum(src_alpha), GLenum(dst_alpha));

	// store the state change
	if (_parent == nullptr ? (function == _default_blending_factors) : (function == _parent->_get_blend_function())) {
		_blending_factors.reset();
	} else {
		_blending_factors = function;
	}
}

void ContextFunctions::SetLogicOp(LogicOp opcode) {
	// check if a state change is necessary
	if (_get_logic_op() == opcode) {
		return;
	}

	// perform the state change
	glLogicOp(GLenum(opcode));

	// store the state change
	if (_parent == nullptr ? (opcode == _default_logic_op) : (opcode == _parent->_get_logic_op())) {
		_logic_op.reset();
	} else {
		_logic_op = opcode;
	}
}

void ContextFunctions::SetDepthFunction(CompareFunction func) {
	// check if a state change is necessary
	if (_get_depth_function() == func) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(func));

	// store the state change
	if (_parent == nullptr ? (func == _default_depth_function) : (func == _parent->_get_depth_function())) {
		_depth_function.reset();
	} else {
		_depth_function = func;
	}
}

void ContextFunctions::SetCullFace(CullFace mode) {
	// check if a state change is necessary
	if (_get_cull_face() == mode) {
		return;
	}

	// perform the state change
	glDepthFunc(GLenum(mode));

	// store the state change
	if (_parent == nullptr ? (mode == _default_cull_face) : (mode == _parent->_get_cull_face())) {
		_cull_face.reset();
	} else {
		_cull_face = mode;
	}
}

void ContextFunctions::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	auto mask = std::make_tuple(red, green, blue, alpha);

	// check if a state change is necessary
	if (_get_color_mask() == mask) {
		return;
	}

	// perform the state change
	glColorMask(red, green, blue, alpha);

	// store the state change
	if (_parent == nullptr ? (mask == _default_color_mask) : (mask == _parent->_get_color_mask())) {
		_color_mask.reset();
	} else {
		_color_mask = mask;
	}
}

void ContextFunctions::SetDepthMask(bool mask) {
	// check if a state change is necessary
	if (_get_depth_mask() == mask) {
		return;
	}

	// perform the state change
	glDepthMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_depth_mask) : (mask == _parent->_get_depth_mask())) {
		_depth_mask.reset();
	} else {
		_depth_mask = mask;
	}
}

void ContextFunctions::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	auto function = std::make_tuple(func, reference, mask);

	// check if a state change is necessary
	if (_get_stencil_func() == function) {
		return;
	}

	// perform the state change
	glStencilFunc(GLenum(func), reference, mask);

	// store the state change
	if (_parent == nullptr ? (function == _default_stencil_func) : (function == _parent->_get_stencil_func())) {
		_stencil_func.reset();
	} else {
		_stencil_func = function;
	}
}

void ContextFunctions::SetStencilMask(uint8_t mask) {
	// check if a state change is necessary
	if (_get_stencil_mask() == mask) {
		return;
	}

	// perform the state change
	glStencilMask(mask);

	// store the state change
	if (_parent == nullptr ? (mask == _default_stencil_mask) : (mask == _parent->_get_stencil_mask())) {
		_stencil_mask.reset();
	} else {
		_stencil_mask = mask;
	}
}

void ContextFunctions::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	auto op = std::make_tuple(sfail, dpfail, dppass);

	// check if a state change is necessary
	if (_get_stencil_op() == op) {
		return;
	}

	// perform the state change
	glStencilOp(GLenum(sfail), GLenum(dpfail), GLenum(dppass));

	// store the state change
	if (_parent == nullptr ? (op == _default_stencil_op) : (op == _parent->_get_stencil_op())) {
		_stencil_op.reset();
	} else {
		_stencil_op = op;
	}
}

void ContextFunctions::SetScissorTest(int x, int y, unsigned width, unsigned height) {
	auto test = std::make_tuple(x, y, width, height);

	// check if a state change is necessary
	if (_get_scissor_test() == test) {
		return;
	}

	// perform the state change
	glScissor(x, y, width, height);

	// store the state change
	if (_parent == nullptr ? (test == _get_default_scissor_test()) : (test == _parent->_get_scissor_test())) {
		_scissor_test.reset();
	} else {
		_scissor_test = test;
	}
}

#pragma clang diagnostic pop

void ContextFunctions::ResetChanges() {
	if (_clear_color.has_value()) {
		const auto& [red, green, blue, alpha] = _parent == nullptr ? _default_clear_color : _parent->_get_clear_color();
		glClearColor(red, green, blue, alpha);
	}

	if (_blending_factors.has_value()) {
		const auto& [src_rgb, dst_rgb, src_alpha, dst_alpha] = _parent == nullptr ? _default_blending_factors : _parent->_get_blend_function();
		glBlendFuncSeparate(GLenum(src_rgb), GLenum(dst_rgb), GLenum(src_alpha), GLenum(dst_alpha));
	}

	if (_logic_op.has_value()) {
		const auto& opcode = _parent == nullptr ? _default_logic_op : _parent->_get_logic_op();
		glLogicOp(GLenum(opcode));
	}

	if (_depth_function.has_value()) {
		const auto& func = _parent == nullptr ? _default_depth_function : _parent->_get_depth_function();
		glDepthFunc(GLenum(func));
	}

	if (_cull_face.has_value()) {
		const auto& cull_face = _parent == nullptr ? _default_cull_face : _parent->_get_cull_face();
		glCullFace(GLenum(cull_face));
	}

	if (_color_mask.has_value()) {
		const auto& [red, green, blue, alpha] = _parent == nullptr ? _default_color_mask : _parent->_get_color_mask();
		glColorMask(red, green, blue, alpha);
	}

	if (_depth_mask.has_value()) {
		const auto& depth_mask = _parent == nullptr ? _default_depth_mask : _parent->_get_depth_mask();
		glDepthMask(depth_mask);
	}

	if (_stencil_func.has_value()) {
		const auto& [func, ref, mask] = _parent == nullptr ? _default_stencil_func : _parent->_get_stencil_func();
		glStencilFunc(GLenum(func), ref, mask);
	}

	if (_stencil_mask.has_value()) {
		const auto& mask = _parent == nullptr ? _default_stencil_mask : _parent->_get_stencil_mask();
		glStencilMask(mask);
	}

	if (_stencil_op.has_value()) {
		const auto& [sfail, dpfail, dppass] = _parent == nullptr ? _default_stencil_op : _parent->_get_stencil_op();
		glStencilOp(GLenum(sfail), GLenum(dpfail), GLenum(dppass));
	}

	if (_scissor_test.has_value()) {
		const auto& [x, y, width, height] = _parent == nullptr ? _get_default_scissor_test() : _parent->_get_scissor_test();
		glScissor(x, y, width, height);
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
	_scissor_test.reset();
}

std::tuple<float, float, float, float> ContextFunctions::_get_clear_color() const {
	// check the current instance
	if (_clear_color.has_value()) {
		return *_clear_color;
	}

	// check parent
	if (_parent != nullptr) {
		return _parent->_get_clear_color();
	}

	// default
	return _default_clear_color;
}

std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor> ContextFunctions::_get_blend_function() const {
	// check the current instance
	if (_blending_factors.has_value()) {
		return *_blending_factors;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_blend_function();
	}

	// default
	return _default_blending_factors;
}

LogicOp ContextFunctions::_get_logic_op() const {
	// check the current instance
	if (_logic_op.has_value()) {
		return *_logic_op;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_logic_op();
	}

	// default
	return _default_logic_op;
}

CompareFunction ContextFunctions::_get_depth_function() const {
	// check the current instance
	if (_depth_function.has_value()) {
		return *_depth_function;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_depth_function();
	}

	// default
	return _default_depth_function;
}

CullFace ContextFunctions::_get_cull_face() const {
	// check the current instance
	if (_cull_face.has_value()) {
		return *_cull_face;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_cull_face();
	}

	// default
	return _default_cull_face;
}

std::tuple<bool, bool, bool, bool> ContextFunctions::_get_color_mask() const {
	// check the current instance
	if (_color_mask.has_value()) {
		return *_color_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_color_mask();
	}

	// default
	return _default_color_mask;
}

bool ContextFunctions::_get_depth_mask() const {
	// check the current instance
	if (_depth_mask.has_value()) {
		return *_depth_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_depth_mask();
	}

	// default
	return _default_depth_mask;
}

std::tuple<CompareFunction, uint8_t, uint8_t> ContextFunctions::_get_stencil_func() const {
	// check the current instance
	if (_stencil_func.has_value()) {
		return *_stencil_func;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_stencil_func();
	}

	// default
	return _default_stencil_func;
}

uint8_t ContextFunctions::_get_stencil_mask() const {
	// check the current instance
	if (_stencil_mask.has_value()) {
		return *_stencil_mask;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_stencil_mask();
	}

	// default
	return _default_stencil_mask;
}

std::tuple<StencilFunction, StencilFunction, StencilFunction> ContextFunctions::_get_stencil_op() const {
	// check the current instance
	if (_stencil_op.has_value()) {
		return *_stencil_op;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_stencil_op();
	}

	// default
	return _default_stencil_op;
}

std::tuple<int, int, unsigned, unsigned> ContextFunctions::_get_scissor_test() const {
	// check the current instance
	if (_scissor_test.has_value()) {
		return *_scissor_test;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_scissor_test();
	}

	// default
	return _get_default_scissor_test();
}

std::tuple<int, int, unsigned, unsigned> ContextFunctions::_get_default_scissor_test() const {
	uvec2 viewport = _context.GetDefaultViewport();
	return std::make_tuple(0, 0, viewport.x, viewport.y);
}

}
