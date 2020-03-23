/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "State.h"

namespace rheel::GL {

std::stack<State *> State::_global_state_stack;

State::State() :
		_parent(nullptr),
		_bindings(),
		_enables(),
		_functions() {}

State::State(State *parent) :
		_parent(parent),
		_bindings(parent->_bindings),
		_enables(parent->_enables),
		_functions(parent->_functions) {}

void State::PushState() {
	State *cur = _global_state_stack.top();
	_global_state_stack.emplace(new State(cur));
}

void State::PopState() {
	if (_global_state_stack.size() == 1) {
		Log::Error() << "OpenGL stack underflow" << std::endl;
		abort();
	}

	State *cur = _global_state_stack.top();
	_global_state_stack.pop();

	cur->_ResetChanges();
	delete cur;
}

void State::Enable(Capability cap) {
	_S()->_enables.Enable(cap);
}

void State::Disable(Capability cap) {
	_S()->_enables.Disable(cap);
}

void State::BindBuffer(const Buffer& buffer) {
	_S()->_bindings.BindBuffer(buffer);
}

void State::BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer) {
	_S()->_bindings.BindFramebuffer(target, framebuffer);
}

void State::BindRenderbuffer(const Renderbuffer& renderbuffer) {
	_S()->_bindings.BindRenderbuffer(renderbuffer);
}

void State::BindTexture(unsigned unit, const Texture& texture) {
	_S()->_bindings.BindTexture(unit, texture);
}

void State::BindVertexArray(const VertexArray& vertexArray) {
	_S()->_bindings.BindVertexArray(vertexArray);
}

void State::UseProgram(const Program& program) {
	_S()->_bindings.UseProgram(program);
}

void State::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	_S()->_functions.SetBlendFunction(sfactor, dfactor);
}

void State::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	_S()->_functions.SetBlendFunction(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void State::_ResetChanges() {
	_bindings.ResetChanges();
	_enables.ResetChanges();
	_functions.ResetChanges();
}

State *State::_S() {
	return _global_state_stack.top();
}

}