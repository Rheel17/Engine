#include "DisplayConfiguration.h"

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

}
