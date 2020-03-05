/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "DisplayConfiguration.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace rheel {

const DisplayConfiguration::Resolution DisplayConfiguration::RESOLUTION_NATIVE { -1, -1 };

unsigned DisplayConfiguration::SampleCount() const {
	switch (aa_mode) {
		case MSAA_4: return 4;
		case MSAA_8: return 8;
		case MSAA_16: return 16;
		default: return 1;
	}
}

void DisplayConfiguration::_CalculateActualResolution() {
	// get the monitor on which to show the window
	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();

	// get the correct resolution.
	int resolutionWidth = resolution.width;
	int resolutionHeight = resolution.height;

	if (resolutionWidth <= 0 || resolutionHeight <= 0) {
		const GLFWvidmode *vidmode = glfwGetVideoMode(primaryMonitor);

		resolutionWidth = vidmode->width;
		resolutionHeight = vidmode->height;
	}

	resolution.width = resolutionWidth;
	resolution.height = resolutionHeight;
}

void DisplayConfiguration::_ClampAnisotropicLevel() {
	if (anisotropic_level < 1.0f) {
		Log::Warning() << "Anisotropic level too low: " << anisotropic_level <<
				" (min: 1.0). Automatically set to min." << std::endl;

		anisotropic_level = 1.0f;
	} else if (anisotropic_level > 0.0f) {
		float max;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);

		if (anisotropic_level > max) {
			if (anisotropic_level != ANISOTROPIC_LEVEL_MAX) {
				Log::Warning() << "Anisotropic level too high: " << anisotropic_level << " (max: " << max <<
						"). Automatically set to max." << std::endl;
			}

			anisotropic_level = max;
		}
	}
}

}
