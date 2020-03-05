/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef DISPLAYCONFIGURATION_H_
#define DISPLAYCONFIGURATION_H_
#include "../_common.h"

namespace rheel {

/**
 * A structure holding the display configuration for the window
 */
struct RE_API DisplayConfiguration {
	friend class Engine;

public:
	enum WindowMode { FULLSCREEN, WINDOWED_UNRESIZABLE };
	enum AntiAliasing { AA_OFF, MSAA_4, MSAA_8, MSAA_16 };
	enum ShadowQuality { SHADOW_OFF, SHADOW_LOW, SHADOW_MEDIUM, SHADOW_HIGH };

	struct Resolution {
		int width;
		int height;
	};

	static constexpr float ANISOTROPIC_LEVEL_MAX = std::numeric_limits<float>::infinity();

public:
	unsigned SampleCount() const;

	WindowMode window_mode = FULLSCREEN;
	Resolution resolution = RESOLUTION_NATIVE;
	std::string title = "Rheel Game Engine";
	bool vsync = true;
	AntiAliasing aa_mode = AA_OFF;
	ShadowQuality shadow_quality = SHADOW_OFF;
	bool enable_mipmaps = false;
	float anisotropic_level = 1.0f;

private:
	void _CalculateActualResolution();
	void _ClampAnisotropicLevel();

public:
	static const Resolution RESOLUTION_NATIVE;

};

}

#endif
