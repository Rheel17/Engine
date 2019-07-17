#include "UI.h"

namespace rheel {

std::shared_ptr<Container> UI::GetContainer() {
	return _ui_container;
}

void UI::Draw() const {
	_ui_container->Draw();
}

std::shared_ptr<UI> UI::Create() {
	return std::shared_ptr<UI>(new UI());
}

}
