/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_DISPLAYCONFIGURATION_H
#define RHEELENGINE_DISPLAYCONFIGURATION_H
#include "../../_common.h"

namespace rheel {

/**
 * A structure holding the display configuration for the window
 */
class RE_API DisplayConfiguration {
	friend class Game;

public:
	enum WindowMode { FULLSCREEN, WINDOWED_UNRESIZABLE, BORDERLESS_WINDOW };
	enum AntiAliasing { AA_OFF, MSAA_4, MSAA_8, MSAA_16 };
	enum ShadowQuality { SHADOW_OFF, SHADOW_LOW, SHADOW_MEDIUM, SHADOW_HIGH };

	static constexpr auto anisotropic_level_max = std::numeric_limits<float>::infinity();

public:
	unsigned SampleCount() const;

	WindowMode window_mode = FULLSCREEN;
	ivec2 resolution = resolution_native;
	bool vsync = true;
	AntiAliasing aa_mode = AA_OFF;
	ShadowQuality shadow_quality = SHADOW_OFF;
	bool enable_mipmaps = false;
	float anisotropic_level = 1.0f;

private:
	void _calculate_actual_resolution();
	void _clamp_anisotropic_level();

public:
	static void InitializeGlfw();
	static void TerminateGlfw();
	static const DisplayConfiguration& Get();

	static const ivec2 resolution_native;

private:
	static void _set(const DisplayConfiguration& display_configuration);

	static DisplayConfiguration _display_configuration;

};

}

#endif
