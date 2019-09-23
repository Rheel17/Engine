#include "Script.h"

namespace rheel {

void Script::SetActive(bool active) {
	_active = active;

	if (!_active) {
		_got_mouse = false;
	}
}

bool Script::IsActive() const {
	return _active;
}

}
