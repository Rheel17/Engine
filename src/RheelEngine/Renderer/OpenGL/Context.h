/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONTEXT_H
#define RHEELENGINE_CONTEXT_H
#include "../../_common.h"

#include <stack>

#include "ContextBindings.h"
#include "ContextEnables.h"
#include "ContextFunctions.h"
#include "VertexArray.h"
#include "Program.h"

namespace rheel {
class Window;
}

namespace rheel::gl {

class RE_API Context {
	friend class Uniform;
	friend class rheel::Window;
	friend class ContextBindings;

	RE_NO_COPY(Context);

private:
	class ContextImpl {

	public:
		ContextImpl(Context& context);
		ContextImpl(ContextImpl* parent);
		~ContextImpl();

		ContextImpl* parent = nullptr;
		ContextBindings bindings;
		ContextEnables enables;
		ContextFunctions functions;

	};

public:
	~Context() noexcept;

	Context(Context&&) noexcept = default;
	Context& operator=(Context&&) noexcept = default;

	uvec2 GetDefaultViewport() const;

	void Push();
	void Pop();
	void CheckStackConsistency();

	void Enable(Capability cap);
	void Disable(Capability cap);

	void BindBuffer(const Buffer& buffer);
	void BindFramebuffer(Framebuffer::Target target, const Framebuffer& framebuffer);
	void BindRenderbuffer(const Renderbuffer& renderbuffer);
	void BindTexture(unsigned unit, const Texture& texture);
	void BindVertexArray(const VertexArray& vertex_array);
	void UseProgram(const Program& program);

	void ClearTexture(unsigned unit, Texture::Target target);
	void ClearProgram();

	void SetClearColor(float red, float green, float blue, float alpha);
	void SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	void SetBlendFunction(BlendFactor src_rgb, BlendFactor dst_rgb, BlendFactor src_alpha, BlendFactor dst_alpha);
	void SetLogicOp(LogicOp opcode);
	void SetDepthFunction(CompareFunction func);
	void SetCullFace(CullFace mode);
	void SetColorMask(bool red, bool green, bool blue, bool alpha);
	void SetDepthMask(bool mask);
	void SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask);
	void SetStencilMask(uint8_t mask);
	void SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass);
	void SetScissorTest(int x, int y, unsigned width, unsigned height);

private:
	explicit Context(uvec2 default_viewport);

	/* Used by uniforms to set their program */
	void _use_program(GLuint handle);

	void _set_active_texture_unit(unsigned unit);

	std::stack<std::unique_ptr<ContextImpl>> _context_stack;
	uvec2 _default_viewport;

	// It doesn't really make sense to make this ContextImpl-local, since it is
	// just meta state
	unsigned _current_active_texture = 0;

public:
	static Context& Current();

};

/**
 * A RAII class for context pushing/popping
 */
class RE_API ContextScope {
	RE_NO_COPY(ContextScope);
	RE_NO_MOVE(ContextScope);

public:
	/**
	 * Pushes the current context.
	 */
	ContextScope();

	/**
	 * Pushes the supplied context.
	 */
	ContextScope(Context& context);

	/**
	 * Pops the context
	 */
	~ContextScope();

private:
	Context& _context;

};

}

#endif
