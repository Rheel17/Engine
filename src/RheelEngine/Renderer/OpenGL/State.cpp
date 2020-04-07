/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "State.h"

namespace rheel::GL {

std::stack<std::unique_ptr<State>> State::_global_state_stack;

State::State() :
		_parent(nullptr),
		_bindings(),
		_enables(),
		_functions() {}

State::State(State *parent) :
		_parent(parent),
		_bindings(&parent->_bindings),
		_enables(&parent->_enables),
		_functions(&parent->_functions) {}

void State::Push() {
	auto& cur = _global_state_stack.top();
	_global_state_stack.emplace(new State(cur.get()));
}

void State::Pop() {
	if (_global_state_stack.size() == 1) {
		Log::Error() << "OpenGL stack underflow" << std::endl;
		abort();
	}

	auto& cur = _global_state_stack.top();
	cur->_ResetChanges();

	_global_state_stack.pop();
}

void State::Enable(Capability cap) {
	_S()._enables.Enable(cap);
}

void State::Disable(Capability cap) {
	_S()._enables.Disable(cap);
}

void State::BindBuffer(const Buffer& buffer) {
	_S()._bindings.BindBuffer(buffer.GetTarget(), buffer.GetName());
}

void State::BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer) {
	_S()._bindings.BindFramebuffer(target, framebuffer.GetName(), framebuffer.GetViewportWidth(), framebuffer.GetViewportHeight());
}

void State::BindRenderbuffer(const Renderbuffer& renderbuffer) {
	_S()._bindings.BindRenderbuffer(renderbuffer.GetName());
}

void State::BindTexture(unsigned unit, const Texture& texture) {
	_S()._bindings.BindTexture(unit, texture.GetTarget(), texture.GetName());
}

void State::BindVertexArray(const VertexArray& vertexArray) {
	_S()._bindings.BindVertexArray(vertexArray.GetName());
}

void State::UseProgram(const Program& program) {
	_S()._bindings.UseProgram(program.GetHandle());
}

void State::ClearTexture(unsigned unit, Texture::Target target) {
	_S()._bindings.BindTexture(unit, target, 0);
}

void State::ClearProgram() {
	_S()._bindings.UseProgram(0);
}

void State::SetClearColor(float red, float green, float blue, float alpha) {
	_S()._functions.SetClearColor(red, green, blue, alpha);
}

void State::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	_S()._functions.SetBlendFunction(sfactor, dfactor);
}

void State::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	_S()._functions.SetBlendFunction(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void State::SetDepthFunction(CompareFunction func) {
	_S()._functions.SetDepthFunction(func);
}

void State::SetCullFace(CullFace mode) {
	_S()._functions.SetCullFace(mode);
}

void State::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	_S()._functions.SetColorMask(red, green, blue, alpha);
}

void State::SetDepthMask(bool mask) {
	_S()._functions.SetDepthMask(mask);
}

void State::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	_S()._functions.SetStencilFunc(func, reference, mask);
}

void State::SetStencilMask(uint8_t mask) {
	_S()._functions.SetStencilMask(mask);
}

void State::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	_S()._functions.SetStencilOp(sfail, dpfail, dppass);
}

void State::_ResetChanges() {
	_bindings.ResetChanges();
	_enables.ResetChanges();
	_functions.ResetChanges();
}

void State::Initialize() {
	_global_state_stack.push(std::unique_ptr<State>(new State));
}

State& State::_S() {
	return *_global_state_stack.top();
}

}