#ifndef INPUT_H_
#define INPUT_H_
#include "../_common.h"

// Include only GLFW, not OpenGL. We're only using the constants.
#ifndef _glfw3_h_
#ifdef GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE
#endif
#endif

#include <bitset>

namespace rheel {

class RE_API Input {

public:
	enum class Action {
		PRESS = GLFW_PRESS, REPEAT = GLFW_REPEAT, RELEASE = GLFW_RELEASE
	};

	using Modifiers = std::bitset<6>;
};

}

#endif
