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
#include "VertexArray.h"
#include "Program.h"

namespace rheel {

class Window;

}

namespace rheel::GL {

class RE_API State {
	friend class Uniform;
	friend class rheel::Window;

	RE_NO_MOVE(State);
	RE_NO_COPY(State);

public:
	static void Push();
	static void Pop();

	static void Enable(Capability cap);
	static void Disable(Capability cap);

	static void BindBuffer(const Buffer& buffer);
	static void BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer);
	static void BindRenderbuffer(const Renderbuffer& renderbuffer);
	static void BindTexture(unsigned unit, const Texture& texture);
	static void BindVertexArray(const VertexArray& vertexArray);
	static void UseProgram(const Program& program);

	static void ClearBuffer(Buffer::Target target);
	static void ClearFramebuffer(Framebuffer::Target target);
	static void ClearRenderbuffer();
	static void ClearTexture(unsigned unit, Texture::Target target);
	static void ClearVertexArray();
	static void ClearProgram();

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
	static void _Initialize();
	static State& _S();

	static std::stack<std::unique_ptr<State>> _global_state_stack;

};

}

#endif
