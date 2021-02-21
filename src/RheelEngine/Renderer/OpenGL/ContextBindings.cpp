/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ContextBindings.h"
#include "Context.h"

namespace rheel::gl {

ContextBindings::ContextBindings(Context& context) :
		_context(context),
		_parent(nullptr) {}

ContextBindings::ContextBindings(const ContextBindings* parent) :
		_context(parent->_context),
		_parent(parent) {}

void ContextBindings::BindBuffer(Buffer::Target target, GLuint name) {
	// check that a state change is necessary
	if (_get_buffer(target) == name) {
		return;
	}

	// perform the state change
	glBindBuffer(GLenum(target), name);

	// store the state change
	if (_parent == nullptr ? (name == 0) : (name == _parent->_get_buffer(target))) {
		_buffer_changes.erase(target);
	} else {
		_buffer_changes[target] = name;
	}
}

void ContextBindings::BindFramebuffer(Framebuffer::Target target, GLuint name, unsigned width, unsigned height) {
	// get targets
	std::vector<Framebuffer::Target> targets{};

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

	// check if a state change is necessary
	if (_get_framebuffer(target) == name) {
		return;
	}

	// perform the state change
	glBindFramebuffer(GLenum(target), name);

	if (target == Framebuffer::Target::DRAW) {
		_set_viewport({ width, height });
	}

	// store the state change
	if (_parent == nullptr ? (name == 0) : name == _parent->_get_framebuffer(target)) {
		_framebuffer_changes.erase(target);
	} else {
		_framebuffer_changes[target] = name;
	}
}

void ContextBindings::BindRenderbuffer(GLuint name) {
	// check if a state change is necessary
	if (_get_renderbuffer() == name) {
		return;
	}

	// perform the state change
	glBindRenderbuffer(GL_RENDERBUFFER, name);

	// store the state change
	if (_parent == nullptr ? (name == 0) : (name == _parent->_get_renderbuffer())) {
		_renderbuffer_change.reset();
	} else {
		_renderbuffer_change = name;
	}
}

void ContextBindings::BindTexture(unsigned unit, Texture::Target target, GLuint name) {
	auto key = std::make_pair(unit, target);

	// check if a state change is necessary
	if (_get_texture(unit, target) == name) {
		return;
	}

	// perform the state change
	_context._set_active_texture_unit(unit);
	glBindTexture(GLenum(target), name);

	if (_parent == nullptr ? (name == 0) : name == _parent->_get_texture(unit, target)) {
		_texture_changes.erase(key);
	} else {
		_texture_changes[key] = name;
	}
}

void ContextBindings::BindVertexArray(GLuint name) {
	// check if a state change is necessary
	if (_get_vertex_array() == name) {
		return;
	}

	// perform the state change
	glBindVertexArray(name);

	if (_parent == nullptr ? name == 0 : name == _parent->_get_vertex_array()) {
		_vertex_array_change.reset();
	} else {
		_vertex_array_change = name;
	}
}

void ContextBindings::UseProgram(GLuint handle) {
	// check if a state change is necessary
	if (_get_program() == handle) {
		return;
	}

	// perform the state change
	glUseProgram(handle);

	if (_parent == nullptr ? handle == 0 : handle == _parent->_get_program()) {
		_program_change.reset();
	} else {
		_program_change = handle;
	}
}

void ContextBindings::ResetChanges() {
	for (const auto& [target, name] : _buffer_changes) {
		glBindBuffer(GLenum(target), _parent == nullptr ? 0 : _parent->_get_buffer(target));
	}

	for (const auto& [target, name] : _framebuffer_changes) {
		glBindFramebuffer(GLenum(target), _parent == nullptr ? 0 : _parent->_get_framebuffer(target));
	}

	if (_viewport_change.has_value()) {
		const auto& viewport = _parent == nullptr ? Framebuffer::DefaultViewport() : _parent->_get_viewport();
		glViewport(0, 0, viewport.x, viewport.y);
	}

	if (_renderbuffer_change.has_value()) {
		glBindRenderbuffer(GL_RENDERBUFFER, _parent == nullptr ? 0 : _parent->_get_renderbuffer());
	}

	for (const auto& [pair, name] : _texture_changes) {
		const auto& [unit, target] = pair;
		_context._set_active_texture_unit(unit);
		glBindTexture(GLenum(target), _parent == nullptr ? 0 : _parent->_get_texture(unit, target));
	}

	if (_vertex_array_change.has_value()) {
		glBindVertexArray(_parent == nullptr ? 0 : _parent->_get_vertex_array());
	}

	if (_program_change.has_value()) {
		glUseProgram(_parent == nullptr ? 0 : _parent->_get_program());
	}

	_buffer_changes.clear();
	_framebuffer_changes.clear();
	_renderbuffer_change.reset();
	_texture_changes.clear();
	_vertex_array_change.reset();
	_program_change.reset();
}

GLuint ContextBindings::_get_buffer(Buffer::Target target) const {
	// check the current instance
	if (auto iter = _buffer_changes.find(target); iter != _buffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_buffer(target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::_get_framebuffer(Framebuffer::Target target) const {
	// check the current instance
	if (auto iter = _framebuffer_changes.find(target); iter != _framebuffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_framebuffer(target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::_get_renderbuffer() const {
	// check the current instance
	if (_renderbuffer_change.has_value()) {
		return *_renderbuffer_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_renderbuffer();
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::_get_texture(unsigned unit, Texture::Target target) const {
	// check the current instance
	if (auto iter = _texture_changes.find({ unit, target }); iter != _texture_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_texture(unit, target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::_get_vertex_array() const {
	// check the current instance
	if (_vertex_array_change.has_value()) {
		return *_vertex_array_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_vertex_array();
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::_get_program() const {
	// check the current instance
	if (_program_change.has_value()) {
		return *_program_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_program();
	}

	// default: no binding
	return 0;
}

void ContextBindings::_set_viewport(uvec2 dim) {
	// check if a state change is necessary
	if (_get_viewport() == dim) {
		return;
	}

	// perform the state change
	glViewport(0, 0, dim.x, dim.y);

	// store the state change
	if (_parent == nullptr ? dim == Framebuffer::DefaultViewport() : dim == _parent->_get_viewport()) {
		_viewport_change.reset();
	} else {
		_viewport_change = dim;
	}
}

uvec2 ContextBindings::_get_viewport() const {
	// check the current instance
	if (_viewport_change.has_value()) {
		return *_viewport_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->_get_viewport();
	}

	// default: no binding
	return Framebuffer::DefaultViewport();
}

}
