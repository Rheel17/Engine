/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CAPABILITIES_H
#define RHEELENGINE_CAPABILITIES_H
#include "../../_common.h"

#include <GL/glwr.h>

namespace rheel::gl {

class RE_API Capabilities {

public:
	static int GetMaxVertexAttribs();

	static float GetMaxTextureMaxAnisotropy();

private:
	static std::unordered_map<GLenum, int> _integer_capabilities;
	static std::unordered_map<GLenum, float> _float_capabilities;

	static int _get_integer(GLenum cap);
	static float _get_float(GLenum cap);

};

}

#endif
