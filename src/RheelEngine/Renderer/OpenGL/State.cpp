/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "State.h"

namespace rheel::gl {

std::stack<std::unique_ptr<State>> State::_global_state_stack;

State::State() :
		_parent(nullptr),
		_bindings(),
		_enables(),
		_functions() {}

State::State(State* parent) :
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
	cur->ResetChanges_();

	_global_state_stack.pop();
}

void State::Enable(Capability cap) {
	S_()._enables.Enable(cap);
}

void State::Disable(Capability cap) {
	S_()._enables.Disable(cap);
}

void State::BindBuffer(const Buffer& buffer) {
	S_()._bindings.BindBuffer(buffer.GetTarget(), buffer.GetName());
}

void State::BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer) {
	S_()._bindings.BindFramebuffer(target, framebuffer.GetName(), framebuffer.GetViewportWidth(), framebuffer.GetViewportHeight());
}

void State::BindRenderbuffer(const Renderbuffer& renderbuffer) {
	S_()._bindings.BindRenderbuffer(renderbuffer.GetName());
}

void State::BindTexture(unsigned unit, const Texture& texture) {
	S_()._bindings.BindTexture(unit, texture.GetTarget(), texture.GetName());
}

void State::BindVertexArray(const VertexArray& vertexArray) {
	S_()._bindings.BindVertexArray(vertexArray.GetName());
}

void State::UseProgram(const Program& program) {
	S_()._bindings.UseProgram(program.GetHandle());
}

void State::ClearTexture(unsigned unit, Texture::Target target) {
	S_()._bindings.BindTexture(unit, target, 0);
}

void State::ClearProgram() {
	S_()._bindings.UseProgram(0);
}

void State::SetClearColor(float red, float green, float blue, float alpha) {
	S_()._functions.SetClearColor(red, green, blue, alpha);
}

void State::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	S_()._functions.SetBlendFunction(sfactor, dfactor);
}

void State::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	S_()._functions.SetBlendFunction(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void State::SetDepthFunction(CompareFunction func) {
	S_()._functions.SetDepthFunction(func);
}

void State::SetCullFace(CullFace mode) {
	S_()._functions.SetCullFace(mode);
}

void State::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	S_()._functions.SetColorMask(red, green, blue, alpha);
}

void State::SetDepthMask(bool mask) {
	S_()._functions.SetDepthMask(mask);
}

void State::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	S_()._functions.SetStencilFunc(func, reference, mask);
}

void State::SetStencilMask(uint8_t mask) {
	S_()._functions.SetStencilMask(mask);
}

void State::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	S_()._functions.SetStencilOp(sfail, dpfail, dppass);
}

void State::ResetChanges_() {
	_bindings.ResetChanges();
	_enables.ResetChanges();
	_functions.ResetChanges();
}

void State::Initialize() {
	_global_state_stack.push(std::unique_ptr<State>(new State));
}

State& State::S_() {
	return *_global_state_stack.top();
}

}