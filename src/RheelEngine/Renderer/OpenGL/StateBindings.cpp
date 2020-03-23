/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateBindings.h"

namespace rheel::GL {

uvec2 StateBindings::_default_viewport;

StateBindings::StateBindings() :
		_parent(nullptr) {}

StateBindings::StateBindings(const StateBindings *parent) :
		_parent(parent) {}

void StateBindings::BindBuffer(const Buffer& buffer) {
	Buffer::Target target = buffer.GetTarget();

	// check if a state change is necessary
	if (_GetBuffer(target) == buffer.GetName()) {
		return;
	}

	// perform the state change
	glBindBuffer(GLenum(target), buffer.GetName());

	// store the state change
	if (_parent->_GetBuffer(target) == buffer.GetName()) {
		// this was a reversal of the current change
		_buffer_changes.erase(target);
	} else {
		// this is a state change
		_buffer_changes.emplace(target, buffer.GetName());
	}
}

void StateBindings::BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer) {
	// get targets
	std::vector<Framebuffer::Target> targets;

	if (target == Framebuffer::Target::BOTH) {
		targets.push_back(Framebuffer::Target::DRAW);
		targets.push_back(Framebuffer::Target::READ);
	} else {
		targets.push_back(target);
	}

	// recurse for multiple targets
	if (targets.size() > 1) {
		for (const auto& t : targets) {
			BindFramebuffer(t, framebuffer);
		}

		return;
	}

	// recursive base case

	// check if a state change is necessary
	if (_GetFramebuffer(target) == framebuffer.GetName()) {
		return;
	}

	// perform the state change
	glBindFramebuffer(GLenum(target), framebuffer.GetName());
	_SetViewport({ framebuffer.GetViewportWidth(), framebuffer.GetViewportHeight() });

	// store the state change
	if (_parent->_GetFramebuffer(target) == framebuffer.GetName()) {
		// this was a reversal of the current change
		_framebuffer_changes.erase(target);
	} else {
		// this is a state change
		_framebuffer_changes.emplace(target, framebuffer.GetName());
	}
}

void StateBindings::BindRenderbuffer(const Renderbuffer& renderbuffer) {
	// check if a state change is necessary
	if (_GetRenderbuffer() == renderbuffer.GetName()) {
		return;
	}

	// perform the state change
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.GetName());

	// store the state change
	if (_parent->_GetRenderbuffer() == renderbuffer.GetName()) {
		// this was a reversal of the current change
		_renderbuffer_change.reset();
	} else {
		// this is a state change
		_renderbuffer_change.emplace(renderbuffer.GetName());
	}
}

void StateBindings::BindTexture(unsigned unit, const Texture& texture) {
	Texture::Target target = texture.GetTarget();
	auto key = std::make_pair(unit, target);

	// check if a state change is necessary
	if (_GetTexture(unit, target) == texture.GetName()) {
		return;
	}

	// perform the state change
	glActiveTexture(unit);
	glBindTexture(GLenum(target), texture.GetName());

	// store the state change
	if (_parent->_GetTexture(unit, target) == texture.GetName()) {
		// this was a reversal of the current change
		_texture_changes.erase(key);
	} else {
		// this is a state change
		_texture_changes.emplace(key, texture.GetName());
	}
}

void StateBindings::BindVertexArray(const VertexArray& vertexArray) {
	// check if a state change is necessary
	if (_GetVertexArray() == vertexArray.GetName()) {
		return;
	}

	// perform the state change
	glBindVertexArray(vertexArray.GetName());

	// store the state change
	if (_parent->_GetVertexArray() == vertexArray.GetName()) {
		// this was a reversal of the current change
		_vertex_array_change.reset();
	} else {
		// this is a state change
		_vertex_array_change.emplace(vertexArray.GetName());
	}
}

void StateBindings::UseProgram(const Program& program) {
	_UseProgram(program.GetHandle());
}

void StateBindings::_UseProgram(GLuint programHandle) {
	// check if a state change is necessary
	if (_GetProgram() == programHandle) {
		return;
	}

	// perform the state change
	glUseProgram(programHandle);

	// store the state change
	if (_parent->_GetProgram() == programHandle) {
		// this was a reversal of the current change
		_vertex_array_change.reset();
	} else {
		// this is a state change
		_vertex_array_change.emplace(programHandle);
	}
}

void StateBindings::ResetChanges() {
	for (const auto& [target, name] : _buffer_changes) {
		glBindBuffer(GLenum(target), _parent->_GetBuffer(target));
	}

	for (const auto& [target, name] : _framebuffer_changes) {
		glBindFramebuffer(GLenum(target), _parent->_GetFramebuffer(target));
	}

	if (_viewport_change.has_value()) {
		const auto& viewport = _parent->_GetViewport();
		glViewport(0, 0, viewport.x, viewport.y);
	}

	if (_renderbuffer_change.has_value()) {
		glBindRenderbuffer(GL_RENDERBUFFER, _parent->_GetRenderbuffer());
	}

	for (const auto& [pair, name] : _texture_changes) {
		const auto& [unit, target] = pair;
		glActiveTexture(unit);
		glBindTexture(GLenum(target), _parent->_GetTexture(unit, target));
	}

	if (_vertex_array_change.has_value()) {
		glBindVertexArray(_parent->_GetVertexArray());
	}

	if (_program_change.has_value()) {
		glUseProgram(_parent->_GetProgram());
	}

	_buffer_changes.clear();
	_framebuffer_changes.clear();
	_renderbuffer_change.reset();
	_texture_changes.clear();
	_vertex_array_change.reset();
	_program_change.reset();
}

GLuint StateBindings::_GetBuffer(Buffer::Target target) const {
	// check the current instance
	if (auto iter = _buffer_changes.find(target); iter != _buffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetBuffer(target);
	}

	// default: no binding
	return 0;
}

GLuint StateBindings::_GetFramebuffer(Framebuffer::Target target) const {
	// check the current instance
	if (auto iter = _framebuffer_changes.find(target); iter != _framebuffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetFramebuffer(target);
	}

	// default: no binding
	return 0;
}

GLuint StateBindings::_GetRenderbuffer() const {
	// check the current instance
	if (_renderbuffer_change.has_value()) {
		return *_renderbuffer_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetRenderbuffer();
	}

	// default: no binding
	return 0;
}

GLuint StateBindings::_GetTexture(unsigned unit, Texture::Target target) const {
	// check the current instance
	if (auto iter = _texture_changes.find({ unit, target }); iter != _texture_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetTexture(unit, target);
	}

	// default: no binding
	return 0;
}

GLuint StateBindings::_GetVertexArray() const {
	// check the current instance
	if (_vertex_array_change.has_value()) {
		return *_vertex_array_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetVertexArray();
	}

	// default: no binding
	return 0;
}

GLuint StateBindings::_GetProgram() const {
	// check the current instance
	if (_program_change.has_value()) {
		return *_program_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetProgram();
	}

	// default: no binding
	return 0;
}

void StateBindings::_SetViewport(uvec2 dim) {
	// check if a state change is necessary
	if (_GetViewport() == dim) {
		return;
	}

	// perform the state change
	glViewport(0, 0, dim.x, dim.y);

	// store the state change
	if (_parent->_GetViewport() == dim) {
		// this was a reversal of the current change
		_viewport_change.reset();
	} else {
		// this is a state change
		_viewport_change = dim;
	}
}

uvec2 StateBindings::_GetViewport() const {
	// check the current instance
	if (_viewport_change.has_value()) {
		return *_viewport_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_GetViewport();
	}

	// default: no binding
	return _default_viewport;
}

void StateBindings::_SetDefaultViewport(unsigned width, unsigned height) {
	_default_viewport = { width, height };
}

}
