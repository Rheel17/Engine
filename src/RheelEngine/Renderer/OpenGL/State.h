/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_STATE_H
#define RHEELENGINE_STATE_H
#include "../../_common.h"

#include <stack>

#include "StateBindings.h"
#include "StateEnables.h"
#include "StateFunctions.h"
#include "VertexArray.h"
#include "Program.h"

namespace rheel::gl {

class RE_API State {
	friend class Uniform;

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

	static void ClearTexture(unsigned unit, Texture::Target target);
	static void ClearProgram();

	static void SetClearColor(float red, float green, float blue, float alpha);
	static void SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	static void SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);
	static void SetDepthFunction(CompareFunction func);
	static void SetCullFace(CullFace mode);
	static void SetColorMask(bool red, bool green, bool blue, bool alpha);
	static void SetDepthMask(bool mask);
	static void SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask);
	static void SetStencilMask(uint8_t mask);
	static void SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass);

private:
	State* _parent;
	StateBindings _bindings;
	StateEnables _enables;
	StateFunctions _functions;

	State();
	explicit State(State* parent);

	/**
	 * Resets the OpenGL state back to its parent state
	 */
	void ResetChanges_();

public:
	static void Initialize();
	static void CheckStackConsistency();

private:
	static State& S_();

	static std::stack<std::unique_ptr<State>> _global_state_stack;

};

}

#endif
