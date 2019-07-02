#ifndef DISPLAYCONFIGURATION_H_
#define DISPLAYCONFIGURATION_H_
#include "../_common.h"

#include <string>

namespace rheel {

/**
 * A structure holding the display configuration for the window
 */
struct RE_API DisplayConfiguration {
	friend class Engine;

public:
	enum WindowMode { FULLSCREEN, WINDOWED_UNRESIZABLE };
	enum AntiAliasing { AA_OFF, MSAA_4, MSAA_8, MSAA_16 };

	struct Resolution {
		int width;
		int height;
	};

public:
	unsigned SampleCount() const;

	WindowMode window_mode = FULLSCREEN;
	Resolution resolution = RESOLUTION_NATIVE;
	std::string title = "Rheel Game Engine";
	bool vsync = true;
	AntiAliasing aa_mode = AA_OFF;

private:
	void _CalculateActualResolution();

public:
	static const Resolution RESOLUTION_NATIVE;

};

}

#endif
