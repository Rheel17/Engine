/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StateEnables.h"

namespace rheel {

std::unordered_map<Capability, bool> StateEnables::_defaults;

StateEnables::StateEnables() :
		_parent(nullptr) {}

StateEnables::StateEnables(const StateEnables *parent) :
		_parent(parent) {}

void StateEnables::Enable(Capability cap) {
	if (!IsEnabled(cap)) {
		glEnable(GLenum(cap));

		// modify the state changes: if the last disable for this capability
		// came from this instance, remove it. If it came from a different
		// instance, add it to this instance.
		if (_state_changes.find(cap) == _state_changes.end()) {
			_state_changes[cap] = true;
		} else {
			_state_changes.erase(cap);
		}
	}
}

void StateEnables::Disable(Capability cap) {
	if (IsEnabled(cap)) {
		glDisable(GLenum(cap));

		// modify the state changes: if the last enable for this capability
		// came from this instance, remove it. If it came from a different
		// instance, add it to this instance.
		if (_state_changes.find(cap) == _state_changes.end()) {
			_state_changes[cap] = false;
		} else {
			_state_changes.erase(cap);
		}
	}
}

bool StateEnables::IsEnabled(Capability cap) const {
	// check whether we had a relevant state change in the current instance
	if (auto iter = _state_changes.find(cap); iter != _state_changes.end()) {
		return iter->second;
	}

	// forward the query to the parent, if it exists
	if (_parent != nullptr) {
		return _parent->IsEnabled(cap);
	}

	// actually get the value from OpenGL, and store it in the defaults.
	auto iter = _defaults.find(cap);
	if (iter == _defaults.end()) {
		iter = _defaults.emplace(cap, glIsEnabled(GLenum(cap))).first;
	}
	return iter->second;
}

void StateEnables::ResetChanges() {
	for (const auto& [cap, enabled] : _state_changes) {
		if (enabled) {
			glDisable(GLenum(cap));
		} else {
			glEnable(GLenum(cap));
		}
	}

	_state_changes.clear();
}

}
