/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "DisplayConfiguration.h"

#include "../OpenGL/Capabilities.h"
#include "Window.h"

namespace rheel {

const ivec2 DisplayConfiguration::resolution_native{ -1, -1 };
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

void DisplayConfiguration::_calculate_actual_resolution() {
	// get the monitor on which to show the window
	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

	// get the correct resolution.
	int resolution_width = resolution.x;
	int resolution_height = resolution.y;

	if (resolution_width <= 0 || resolution_height <= 0) {
		const GLFWvidmode* vidmode = glfwGetVideoMode(primary_monitor);

		resolution_width = vidmode->width;
		resolution_height = vidmode->height;
	}

	resolution.x = resolution_width;
	resolution.y = resolution_height;
}

void DisplayConfiguration::_clamp_anisotropic_level() {
	if (anisotropic_level < 1.0f) {
		Log::Warning() << "Anisotropic level too low: " << anisotropic_level << " (min: 1.0). Automatically set to min." << std::endl;
		anisotropic_level = 1.0f;
	} else if (anisotropic_level > 0.0f) {
		float max = gl::Capabilities::GetMaxTextureMaxAnisotropy();

		if (anisotropic_level > max) {
			if (anisotropic_level != anisotropic_level_max) {
				Log::Warning() << "Anisotropic level too high: " << anisotropic_level << " (max: " << max << "). Automatically set to max." << std::endl;
			}

			anisotropic_level = max;
		}
	}
}

const DisplayConfiguration& DisplayConfiguration::Get() {
	return _display_configuration;
}

void DisplayConfiguration::InitializeGlfw() {
	glfwSetErrorCallback([](int code, const char* description) {
		Log::Error() << "GLFW " << code << ": " << description << std::endl;
	});

	if (!glfwInit()) {
		Log::Error() << "Unable to initialize GLFW" << std::endl;
		abort();
	}
}

void DisplayConfiguration::TerminateGlfw() {
	glfwTerminate();
}

void DisplayConfiguration::_set(const DisplayConfiguration& display_configuration) {
	_display_configuration = display_configuration;
}

}
