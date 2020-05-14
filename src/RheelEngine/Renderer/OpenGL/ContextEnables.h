/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONTEXTENABLES_H
#define RHEELENGINE_CONTEXTENABLES_H
#include "../../_common.h"

#include <GL/glew.h>

namespace rheel::gl {

enum class Capability {
	BLEND = GL_BLEND,
	COLOR_LOGIC_OP = GL_COLOR_LOGIC_OP,
	DEPTH_TEST = GL_DEPTH_TEST,
	STENCIL_TEST = GL_STENCIL_TEST,
	SCISSOR_TEST = GL_SCISSOR_TEST,
	CULL_FACE = GL_CULL_FACE,
};

class RE_API ContextEnables {

public:

public:
	ContextEnables();
	explicit ContextEnables(const ContextEnables* parent);

	void Enable(Capability cap);
	void Disable(Capability cap);

	bool IsEnabled(Capability cap) const;

	void ResetChanges();

private:
	std::unordered_map<Capability, bool> _state_changes;
	const ContextEnables* _parent;

private:
	static std::unordered_map<Capability, bool> _defaults;

};

}

#endif
