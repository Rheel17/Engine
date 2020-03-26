/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_STATEENABLED_H
#define RHEELENGINE_GL_STATEENABLED_H
#include "../../_common.h"

#include <GL/glew.h>

namespace rheel::GL {

enum class Capability {
	BLEND = GL_BLEND, STENCIL_TEST = GL_STENCIL_TEST
};

class RE_API StateEnables {

public:

public:
	StateEnables();
	explicit StateEnables(const StateEnables *parent);

	void Enable(Capability cap);
	void Disable(Capability cap);

	bool IsEnabled(Capability cap) const;

	void ResetChanges();

private:
	std::unordered_map<Capability, bool> _state_changes;
	const StateEnables *_parent;

private:
	static std::unordered_map<Capability, bool> _defaults;

};

}

#endif
