#include "Input.h"

namespace rheel {

Input::Scancode Input::GetScancode(Key key) {
	return glfwGetKeyScancode(static_cast<int>(key));
}

}
