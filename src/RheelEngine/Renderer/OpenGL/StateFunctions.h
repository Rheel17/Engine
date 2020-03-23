/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_STATEFUNCTIONS_H
#define RHEELENGINE_GL_STATEFUNCTIONS_H
#include "../../_common.h"

#include <gl/glew.h>

namespace rheel::GL {

enum class BlendFactor {
	ZERO = GL_ZERO, ONE = GL_ONE,
	SRC_COLOR = GL_SRC_COLOR, ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	SRC_ALPHA = GL_SRC_ALPHA, ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	DST_COLOR = GL_DST_COLOR, ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	DST_ALPHA = GL_DST_ALPHA, ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
};

class RE_API StateFunctions {

public:
	StateFunctions();
	explicit StateFunctions(StateFunctions *parent);

	// blending
	void SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	void SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);

	void ResetChanges();

private:
	std::tuple<GLenum, GLenum, GLenum, GLenum> _GetBlendFunction() const;

	std::optional<std::tuple<GLenum, GLenum, GLenum, GLenum>> _blending_factors;

	StateFunctions *_parent;

};

}

#endif
