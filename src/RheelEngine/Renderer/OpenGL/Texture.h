/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_TEXTURE_H
#define RHEELENGINE_TEXTURE_H
#include "../../_common.h"

#include "Object.h"

namespace rheel::gl {

OPENGL_GEN_FUNCTION(glGenTextures, gen_textures_);
OPENGL_DELETE_FUNCTION(glDeleteTextures, delete_textures_);

class RE_API Texture : public Object<gen_textures_, delete_textures_> {

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

	enum class CompareMode {
		COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE, NONE = GL_NONE
	};

public:
	explicit Texture(Target target);

	void Bind(unsigned int textureUnit = 0) const;

	Target GetTarget() const;

private:
	Target _target;

};

}

#endif
