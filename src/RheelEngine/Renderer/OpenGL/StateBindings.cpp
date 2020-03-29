/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateBindings.h"

namespace rheel::GL {

StateBindings::StateBindings() :
		_parent(nullptr) {}

StateBindings::StateBindings(const StateBindings *parent) :
		_parent(parent) {}

// TODO: correctly prevent useless state changes. Ideally also
//  id -> 0 -> id
//  replaced by
//  id
//  if no bindings of other objects happen in between. Idea: use
//  some kind of 'flushing' system.

void StateBindings::BindBuffer(Buffer::Target target, GLuint name) {
	// perform the state change
	glBindBuffer(GLenum(target), name);
	_buffer_changes[target] = name;
}

void StateBindings::BindFramebuffer(Framebuffer::Target target, GLuint name, unsigned width, unsigned height) {
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
			BindFramebuffer(t, name, width, height);
		}

		return;
	}

	// recursive base case

	// perform the state change
	glBindFramebuffer(GLenum(target), name);

	if (target == Framebuffer::Target::DRAW) {
		_SetViewport({ width, height });
	}

	_framebuffer_changes[target] = name;
}

void StateBindings::BindRenderbuffer(GLuint name) {
	// perform the state change
	glBindRenderbuffer(GL_RENDERBUFFER, name);
	_renderbuffer_change = name;
}

void StateBindings::BindTexture(unsigned unit, Texture::Target target, GLuint name) {
	auto key = std::make_pair(unit, target);

	// perform the state change
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GLenum(target), name);
	_texture_changes[key] = name;
}

void StateBindings::BindVertexArray(GLuint name) {
	glBindVertexArray(name);
	_vertex_array_change = name;
}

void StateBindings::UseProgram(GLuint handle) {
	glUseProgram(handle);
	_program_change = handle;
}

void StateBindings::ResetChanges() {
	for (const auto& [target, name] : _buffer_changes) {
		glBindBuffer(GLenum(target), _parent == nullptr ? 0 : _parent->_GetBuffer(target));
	}

	for (const auto& [target, name] : _framebuffer_changes) {
		glBindFramebuffer(GLenum(target), _parent == nullptr ? 0 : _parent->_GetFramebuffer(target));
	}

	if (_viewport_change.has_value()) {
		const auto& viewport = _parent == nullptr ? Framebuffer::DefaultViewport() : _parent->_GetViewport();
		glViewport(0, 0, viewport.x, viewport.y);
	}

	if (_renderbuffer_change.has_value()) {
		glBindRenderbuffer(GL_RENDERBUFFER, _parent == nullptr ? 0 : _parent->_GetRenderbuffer());
	}

	for (const auto& [pair, name] : _texture_changes) {
		const auto& [unit, target] = pair;
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GLenum(target), _parent == nullptr ? 0 : _parent->_GetTexture(unit, target));
	}

	if (_vertex_array_change.has_value()) {
		glBindVertexArray(_parent == nullptr ? 0 : _parent->_GetVertexArray());
	}

	if (_program_change.has_value()) {
		glUseProgram(_parent == nullptr ? 0 : _parent->_GetProgram());
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
	if ((_parent == nullptr && dim == Framebuffer::DefaultViewport()) || (_parent != nullptr && _parent->_GetViewport() == dim)) {
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
	return Framebuffer::DefaultViewport();
}

}
