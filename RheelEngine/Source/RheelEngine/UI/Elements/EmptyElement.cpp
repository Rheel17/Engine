#include "EmptyElement.h"

namespace rheel::ui {

EmptyElement::EmptyElement(unsigned width, unsigned height) :
		_width(width), _height(height) {}

std::pair<unsigned, unsigned> EmptyElement::GetDefaultDimensions() const {
	return std::make_pair(_width, _height);
}

}
