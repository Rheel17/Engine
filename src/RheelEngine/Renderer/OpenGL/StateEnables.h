/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATEENABLES_H
#define RHEELENGINE_STATEENABLES_H
#include "../../_common.h"

#include <GL/glew.h>

namespace rheel::gl {

enum class Capability {
	BLEND = GL_BLEND,
	DEPTH_TEST = GL_DEPTH_TEST,
	STENCIL_TEST = GL_STENCIL_TEST,
	CULL_FACE = GL_CULL_FACE
};

class RE_API StateEnables {

public:

public:
	StateEnables();
	explicit StateEnables(const StateEnables* parent);

	void Enable(Capability cap);
	void Disable(Capability cap);

	bool IsEnabled(Capability cap) const;

	void ResetChanges();

private:
	std::unordered_map<Capability, bool> _state_changes;
	const StateEnables* _parent;

private:
	static std::unordered_map<Capability, bool> _defaults;

};

}

#endif
