/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_TEXTURE_H
#define RHEELENGINE_GL_TEXTURE_H
#include "../../_common.h"

#include "Object.h"

namespace rheel::GL {

OPENGL_GENDEL_FUNCTION(glGenTextures, _GenTextures);
OPENGL_GENDEL_FUNCTION(glDeleteTextures, _DeleteTexture);

class RE_API Texture : public Object<_GenTextures, _DeleteTexture> {

public:
	enum class Target {
		TEXTURE_1D = GL_TEXTURE_1D, TEXTURE_2D = GL_TEXTURE_2D, TEXTURE_3D = GL_TEXTURE_3D,
		TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY, TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE, TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
		TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY, TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
		TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class FilterFunction {
		NEAREST                = GL_NEAREST,
		LINEAR                 = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class WrapParameter {
		CLAMP_TO_EDGE          = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER        = GL_CLAMP_TO_BORDER,
		REPEAT                 = GL_REPEAT,
		MIRRORED_REPEAT        = GL_MIRRORED_REPEAT,
		MIRRORED_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
	};

public:
	explicit Texture(Target target);

	void Bind(unsigned int textureUnit = 0) const;

	Target GetTarget() const;

	/**
	 * Sets the minifying function for this texture.
	 */
	void SetMinifyingFilter(FilterFunction filterFunction);

	/**
	 * Sets the magnification function for this texture.
	 */
	void SetMagnificationFilter(FilterFunction filterFunction);

	/**
	 * Sets the anisotropy parameter
	 */
	void SetAnisotropyParameter(float parameter);

	/**
	 * Generates the mipmap for this texture
	 */
	void GenerateMipmap();

private:
	Target _target;

};

}

#endif
