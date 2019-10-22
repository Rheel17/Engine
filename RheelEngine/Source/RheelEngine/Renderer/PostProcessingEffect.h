///*
// * Copyright © 2019 Levi van Rheenen. All rights reserved.
// */
//#ifndef POSTPROCESSINGEFFECT_H_
//#define POSTPROCESSINGEFFECT_H_
//#include "../_common.h"
//
//#include "OpenGL/GLFramebuffer.h"
//#include "OpenGL/GLVertexArray.h"
//
//namespace rheel {
//
//class PostProcessingStack;
//
//class PostProcessingEffect {
//	friend class PostProcessingStack;
//
//public:
//	virtual ~PostProcessingEffect() = default;
//
//	/**
//	 * Renders this effect
//	 */
//	const virtual GLFramebuffer& Render(const GLFramebuffer& input) const = 0;
//
//protected:
//	/**
//	 * Returns an unused temporary framebuffer index
//	 */
//	unsigned _UnusedFramebufferIndex() const;
//
//	/**
//	 * Returns the index of a temporary framebuffer
//	 */
//	unsigned _GetFramebufferIndex(const GLFramebuffer& buffer) const;
//
//	/**
//	 * Returns the temporary framebuffer at an index
//	 */
//	GLFramebuffer& _Framebuffer(unsigned index) const;
//
//	/**
//	 * Sets the use flag of a temporary framebuffer
//	 */
//	void _MarkFramebufferUse(unsigned index, bool flag) const;
//
//private:
//	PostProcessingStack *_Stack() const;
//
//	PostProcessingStack *_stack;
//
//protected:
//	static void _DrawScreenQuad();
//
//	static std::unique_ptr<GLBuffer> _screen_quad_buffer;
//	static std::unique_ptr<GLVertexArray> _screen_quad;
//
//};
//
//}
//
//#endif
