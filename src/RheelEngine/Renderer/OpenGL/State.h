/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_STATE_H
#define RHEELENGINE_GL_STATE_H
#include "../../_common.h"

#include <stack>

#include "StateBindings.h"
#include "StateEnables.h"
#include "StateFunctions.h"

namespace rheel::GL {

class RE_API State {
	friend class Uniform;

public:
	static void PushState();
	static void PopState();

	static void Enable(Capability cap);
	static void Disable(Capability cap);

	static void BindBuffer(const Buffer& buffer);
	static void BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer);
	static void BindRenderbuffer(const Renderbuffer& renderbuffer);
	static void BindTexture(unsigned unit, const Texture& texture);
	static void BindVertexArray(const VertexArray& vertexArray);
	static void UseProgram(const Program& program);

	static void SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	static void SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);

private:
	State *_parent;
	StateBindings _bindings;
	StateEnables _enables;
	StateFunctions _functions;

	State();
	explicit State(State *parent);

	/**
	 * Resets the OpenGL state back to its parent state
	 */
	void _ResetChanges();

private:
	static std::stack<State *> _global_state_stack;

	static State *_S();


};

}

#endif
