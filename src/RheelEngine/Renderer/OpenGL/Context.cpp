/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Context.h"

#include "../Display/Window.h"

namespace rheel::gl {

Context::ContextImpl::ContextImpl(Context& context) :
		bindings(context),
		enables(),
		functions(context) {}

Context::ContextImpl::ContextImpl(ContextImpl* parent) :
		parent(parent),
		bindings(&parent->bindings),
		enables(&parent->enables),
		functions(&parent->functions) {}

Context::ContextImpl::~ContextImpl() {
	if (parent == nullptr) {
		return;
	}

	bindings.ResetChanges();
	enables.ResetChanges();
	functions.ResetChanges();
}

Context::Context(uvec2 default_viewport) :
		_default_viewport(default_viewport) {

	_context_stack.push(std::make_unique<ContextImpl>(*this));
}

Context::~Context() noexcept {
	CheckStackConsistency();
	_context_stack.pop();
}

uvec2 Context::GetDefaultViewport() const {
	return _default_viewport;
}

void Context::Push() {
	auto& cur = _context_stack.top();
	_context_stack.push(std::make_unique<ContextImpl>(cur.get()));
}

void Context::Pop() {
	if (_context_stack.size() == 1) {
		Log::Error() << "OpenGL stack underflow" << std::endl;
		abort();
	}

	// Destruction of the context implementation takes care of reseting the
	// OpenGL state.
	_context_stack.pop();
}

void Context::CheckStackConsistency() {
	if (_context_stack.size() != 1) {
		Log::Error() << "OpenGL Stack inconsistent state: make sure there are an equal number of push and pop operations" << std::endl;
		abort();
	}
}

void Context::Enable(Capability cap) {
	_context_stack.top()->enables.Enable(cap);
}

void Context::Disable(Capability cap) {
	_context_stack.top()->enables.Disable(cap);
}

void Context::BindBuffer(const Buffer& buffer) {
	_context_stack.top()->bindings.BindBuffer(buffer.GetTarget(), buffer.GetName());
}

void Context::BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer) {
	_context_stack.top()->bindings.BindFramebuffer(target, framebuffer.GetName(), framebuffer.GetViewportWidth(), framebuffer.GetViewportHeight());
}

void Context::BindRenderbuffer(const Renderbuffer& renderbuffer) {
	_context_stack.top()->bindings.BindRenderbuffer(renderbuffer.GetName());
}

void Context::BindTexture(unsigned unit, const Texture& texture) {
	_context_stack.top()->bindings.BindTexture(unit, texture.GetTarget(), texture.GetName());
}

void Context::BindVertexArray(const VertexArray& vertex_array) {
	_context_stack.top()->bindings.BindVertexArray(vertex_array.GetName());
}

void Context::UseProgram(const Program& program) {
	_context_stack.top()->bindings.UseProgram(program.GetHandle());
}

void Context::ClearTexture(unsigned unit, Texture::Target target) {
	_context_stack.top()->bindings.BindTexture(unit, target, 0);
}

void Context::ClearProgram() {
	_context_stack.top()->bindings.UseProgram(0);
}

void Context::SetClearColor(float red, float green, float blue, float alpha) {
	_context_stack.top()->functions.SetClearColor(red, green, blue, alpha);
}

void Context::SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor) {
	_context_stack.top()->functions.SetBlendFunction(sfactor, dfactor);
}

void Context::SetBlendFunction(BlendFactor src_rgb, BlendFactor dst_rgb, BlendFactor src_alpha, BlendFactor dst_alpha) {
	_context_stack.top()->functions.SetBlendFunction(src_rgb, dst_rgb, src_alpha, dst_alpha);
}

void Context::SetLogicOp(LogicOp opcode) {
	_context_stack.top()->functions.SetLogicOp(opcode);
}

void Context::SetDepthFunction(CompareFunction func) {
	_context_stack.top()->functions.SetDepthFunction(func);
}

void Context::SetCullFace(CullFace mode) {
	_context_stack.top()->functions.SetCullFace(mode);
}

void Context::SetColorMask(bool red, bool green, bool blue, bool alpha) {
	_context_stack.top()->functions.SetColorMask(red, green, blue, alpha);
}

void Context::SetDepthMask(bool mask) {
	_context_stack.top()->functions.SetDepthMask(mask);
}

void Context::SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask) {
	_context_stack.top()->functions.SetStencilFunc(func, reference, mask);
}

void Context::SetStencilMask(uint8_t mask) {
	_context_stack.top()->functions.SetStencilMask(mask);
}

void Context::SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass) {
	_context_stack.top()->functions.SetStencilOp(sfail, dpfail, dppass);
}

void Context::SetScissorTest(int x, int y, unsigned int width, unsigned int height) {
	_context_stack.top()->functions.SetScissorTest(x, y, width, height);
}

void Context::_use_program(GLuint handle) {
	_context_stack.top()->bindings.UseProgram(handle);
}

void Context::_set_active_texture_unit(unsigned unit) {
	if (_current_active_texture == unit) {
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	_current_active_texture = unit;
}

Context& Context::Current() {
	GLFWwindow* glfw_window = glfwGetCurrentContext();
	auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
	return window->GetContext();
}

ContextScope::ContextScope() :
		ContextScope(Context::Current()) {}

ContextScope::ContextScope(Context& context) :
		_context(context) {

	_context.Push();
}

ContextScope::~ContextScope() {
	_context.Pop();
}

}