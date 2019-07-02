#ifndef EMPTYELEMENT_H_
#define EMPTYELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel::ui {

class RE_API EmptyElement : public Element {

public:
	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override {}

private:
	EmptyElement(unsigned width, unsigned height);

	unsigned _width, _height;

public:
	/**
	 * Creates an empty element of a certain width and height.
	 */
	inline static std::shared_ptr<EmptyElement> Create(unsigned width, unsigned height) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<EmptyElement>(new EmptyElement(width, height));
	}

};

}

#endif
