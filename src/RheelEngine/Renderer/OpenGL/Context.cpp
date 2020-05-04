/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Context.h"

#include "../Display/Window.h"

namespace rheel::gl {

Context::ContextImpl::ContextImpl(Context& context) :
		bindings(context) {}

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

Context::Context(uvec2 defaultViewport) :
		_default_viewport(defaultViewport) {

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

void Context::BindVertexArray(const VertexArray& vertexArray) {
	_context_stack.top()->bindings.BindVertexArray(vertexArray.GetName());
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

void Context::SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha) {
	_context_stack.top()->functions.SetBlendFunction(srcRGB, dstRGB, srcAlpha, dstAlpha);
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

void Context::UseProgram_(GLuint handle) {
	_context_stack.top()->bindings.UseProgram(handle);
}

void Context::SetActiveTextureUnit_(unsigned unit) {
	if (_current_active_texture == unit) {
		return;
	}

	glActiveTexture(GL_TEXTURE0 + unit);
	_current_active_texture = unit;
}

Context& Context::Current() {
	GLFWwindow* glfwWindow = glfwGetCurrentContext();
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	return window->GetContext();
}

}