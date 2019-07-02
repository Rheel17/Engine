#ifndef COLOR_H_
#define COLOR_H_
#include "../_common.h"

namespace rheel {

struct RE_API Color {
	float r, g, b, a;

	inline operator vec4() const { return { r, g, b, a }; }
};

}

#endif
