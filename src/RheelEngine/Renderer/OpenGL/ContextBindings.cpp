/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "ContextBindings.h"

namespace rheel::gl {

ContextBindings::ContextBindings() :
		_parent(nullptr) {}

ContextBindings::ContextBindings(const ContextBindings* parent) :
		_parent(parent) {}

void ContextBindings::BindBuffer(Buffer::Target target, GLuint name) {
	// check that a state change is necessary
	if (GetBuffer_(target) == name) {
		return;
	}

	// perform the state change
	glBindBuffer(GLenum(target), name);

	// store the state change
	if (_parent == nullptr ? (name == 0) : (name == _parent->GetBuffer_(target))) {
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
	if (GetFramebuffer_(target) == name) {
		return;
	}

	// perform the state change
	glBindFramebuffer(GLenum(target), name);

	if (target == Framebuffer::Target::DRAW) {
		SetViewport_({ width, height });
	}

	// store the state change
	if (_parent == nullptr ? (name == 0) : name == _parent->GetFramebuffer_(target)) {
		_framebuffer_changes.erase(target);
	} else {
		_framebuffer_changes[target] = name;
	}
}

void ContextBindings::BindRenderbuffer(GLuint name) {
	// check if a state change is necessary
	if (GetRenderbuffer_() == name) {
		return;
	}

	// perform the state change
	glBindRenderbuffer(GL_RENDERBUFFER, name);

	// store the state change
	if (_parent == nullptr ? (name == 0) : (name == _parent->GetRenderbuffer_())) {
		_renderbuffer_change.reset();
	} else {
		_renderbuffer_change = name;
	}
}

void ContextBindings::BindTexture(unsigned unit, Texture::Target target, GLuint name) {
	auto key = std::make_pair(unit, target);

	// check if a state change is necessary
	if (GetTexture_(unit, target) == name) {
		return;
	}

	// perform the state change
	SetActiveTextureUnit_(unit);
	glBindTexture(GLenum(target), name);

	if (_parent == nullptr ? (name == 0) : name == _parent->GetTexture_(unit, target)) {
		_texture_changes.erase(key);
	} else {
		_texture_changes[key] = name;
	}
}

void ContextBindings::BindVertexArray(GLuint name) {
	// check if a state change is necessary
	if (GetVertexArray_() == name) {
		return;
	}

	// perform the state change
	glBindVertexArray(name);

	if (_parent == nullptr ? name == 0 : name == _parent->GetVertexArray_()) {
		_vertex_array_change.reset();
	} else {
		_vertex_array_change = name;
	}
}

void ContextBindings::UseProgram(GLuint handle) {
	// check if a state change is necessary
	if (GetProgram_() == handle) {
		return;
	}

	// perform the state change
	glUseProgram(handle);

	if (_parent == nullptr ? handle == 0 : handle == _parent->GetProgram_()) {
		_program_change.reset();
	} else {
		_program_change = handle;
	}
}

void ContextBindings::ResetChanges() {
	for (const auto& [target, name] : _buffer_changes) {
		glBindBuffer(GLenum(target), _parent == nullptr ? 0 : _parent->GetBuffer_(target));
	}

	for (const auto& [target, name] : _framebuffer_changes) {
		glBindFramebuffer(GLenum(target), _parent == nullptr ? 0 : _parent->GetFramebuffer_(target));
	}

	if (_viewport_change.has_value()) {
		const auto& viewport = _parent == nullptr ? Framebuffer::DefaultViewport() : _parent->GetViewport_();
		glViewport(0, 0, viewport.x, viewport.y);
	}

	if (_renderbuffer_change.has_value()) {
		glBindRenderbuffer(GL_RENDERBUFFER, _parent == nullptr ? 0 : _parent->GetRenderbuffer_());
	}

	for (const auto& [pair, name] : _texture_changes) {
		const auto& [unit, target] = pair;
		SetActiveTextureUnit_(unit);
		glBindTexture(GLenum(target), _parent == nullptr ? 0 : _parent->GetTexture_(unit, target));
	}

	if (_vertex_array_change.has_value()) {
		glBindVertexArray(_parent == nullptr ? 0 : _parent->GetVertexArray_());
	}

	if (_program_change.has_value()) {
		glUseProgram(_parent == nullptr ? 0 : _parent->GetProgram_());
	}

	glActiveTexture(GL_TEXTURE0 + (_parent == nullptr ? 0 : _parent->GetActiveTextureUnit_()));

	_buffer_changes.clear();
	_framebuffer_changes.clear();
	_renderbuffer_change.reset();
	_texture_changes.clear();
	_vertex_array_change.reset();
	_program_change.reset();
	_texture_unit_change.reset();
}

GLuint ContextBindings::GetBuffer_(Buffer::Target target) const {
	// check the current instance
	if (auto iter = _buffer_changes.find(target); iter != _buffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetBuffer_(target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::GetFramebuffer_(Framebuffer::Target target) const {
	// check the current instance
	if (auto iter = _framebuffer_changes.find(target); iter != _framebuffer_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetFramebuffer_(target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::GetRenderbuffer_() const {
	// check the current instance
	if (_renderbuffer_change.has_value()) {
		return *_renderbuffer_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetRenderbuffer_();
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::GetTexture_(unsigned unit, Texture::Target target) const {
	// check the current instance
	if (auto iter = _texture_changes.find({ unit, target }); iter != _texture_changes.end()) {
		return iter->second;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetTexture_(unit, target);
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::GetVertexArray_() const {
	// check the current instance
	if (_vertex_array_change.has_value()) {
		return *_vertex_array_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetVertexArray_();
	}

	// default: no binding
	return 0;
}

GLuint ContextBindings::GetProgram_() const {
	// check the current instance
	if (_program_change.has_value()) {
		return *_program_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetProgram_();
	}

	// default: no binding
	return 0;
}

void ContextBindings::SetViewport_(uvec2 dim) {
	// check if a state change is necessary
	if (GetViewport_() == dim) {
		return;
	}

	// perform the state change
	glViewport(0, 0, dim.x, dim.y);

	// store the state change
	if (_parent == nullptr ? dim == Framebuffer::DefaultViewport() : dim == _parent->GetViewport_()) {
		_viewport_change.reset();
	} else {
		_viewport_change = dim;
	}
}

uvec2 ContextBindings::GetViewport_() const {
	// check the current instance
	if (_viewport_change.has_value()) {
		return *_viewport_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetViewport_();
	}

	// default: no binding
	return Framebuffer::DefaultViewport();
}

void ContextBindings::SetActiveTextureUnit_(unsigned unit) {
	// check if a state change is necessary
	if (GetActiveTextureUnit_() == unit) {
		return;
	}

	// perform the state change
	glActiveTexture(GL_TEXTURE0 + unit);

	// store the state change
	if (_parent == nullptr ? unit == 0 : unit == _parent->GetActiveTextureUnit_()) {
		_texture_unit_change.reset();
	} else {
		_texture_unit_change = unit;
	}
}

unsigned ContextBindings::GetActiveTextureUnit_() const {
	// check the current instance
	if (_texture_unit_change.has_value()) {
		return *_texture_unit_change;
	}

	// check the parent
	if (_parent != nullptr) {
		return _parent->GetActiveTextureUnit_();
	}

	// default: no binding
	return 0;
}

}
