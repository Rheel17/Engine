/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "DisplayConfiguration.h"

#include "../OpenGL/Capabilities.h"
#include "Window.h"

namespace rheel {

const ivec2 DisplayConfiguration::RESOLUTION_NATIVE{ -1, -1 };
DisplayConfiguration DisplayConfiguration::_display_configuration;

unsigned DisplayConfiguration::SampleCount() const {
	switch (aa_mode) {
		case MSAA_4:
			return 4;
		case MSAA_8:
			return 8;
		case MSAA_16:
			return 16;
		default:
			return 1;
	}
}

void DisplayConfiguration::CalculateActualResolution_() {
	// get the monitor on which to show the window
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

	// get the correct resolution.
	int resolutionWidth = resolution.x;
	int resolutionHeight = resolution.y;

	if (resolutionWidth <= 0 || resolutionHeight <= 0) {
		const GLFWvidmode* vidmode = glfwGetVideoMode(primaryMonitor);

		resolutionWidth = vidmode->width;
		resolutionHeight = vidmode->height;
	}

	resolution.x = resolutionWidth;
	resolution.y = resolutionHeight;
}

void DisplayConfiguration::ClampAnisotropicLevel_() {
	if (anisotropic_level < 1.0f) {
		Log::Warning() << "Anisotropic level too low: " << anisotropic_level << " (min: 1.0). Automatically set to min." << std::endl;
		anisotropic_level = 1.0f;
	} else if (anisotropic_level > 0.0f) {
		float max = gl::Capabilities::GetMaxTextureMaxAnisotropy();

		if (anisotropic_level > max) {
			if (anisotropic_level != ANISOTROPIC_LEVEL_MAX) {
				Log::Warning() << "Anisotropic level too high: " << anisotropic_level << " (max: " << max << "). Automatically set to max." << std::endl;
			}

			anisotropic_level = max;
		}
	}
}

const DisplayConfiguration& DisplayConfiguration::Get() {
	return _display_configuration;
}

void DisplayConfiguration::InitializeGLFW() {
	glfwSetErrorCallback([](int code, const char* description) {
		Log::Error() << "GLFW " << code << ": " << description << std::endl;
	});

	if (!glfwInit()) {
		Log::Error() << "Unable to initialize GLFW" << std::endl;
		abort();
	}
}

void DisplayConfiguration::TerminateGLFW() {
	glfwTerminate();
}

void DisplayConfiguration::Set_(DisplayConfiguration&& displayConfiguration) {
	_display_configuration = std::move(displayConfiguration);
}

}
