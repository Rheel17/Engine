/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GL_CAPABILITIES_H
#define RHEELENGINE_GL_CAPABILITIES_H
#include "../../_common.h"

#include <gl/glew.h>

namespace rheel::GL {

class RE_API Capabilities {

public:
	static int GetMaxVertexAttribs();

	static float GetMaxTextureMaxAnisotropy();

private:
	static std::unordered_map<GLenum, int> _integer_capabilities;
	static std::unordered_map<GLenum, float> _float_capabilities;

	static int _GetInteger(GLenum cap);
	static float _GetFloat(GLenum cap);

};

}

#endif
