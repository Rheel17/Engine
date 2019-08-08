#ifndef EMPTYELEMENT_H_
#define EMPTYELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API EmptyElement : public Element {

public:
	/**
	 * Creates an empty element of a certain width and height.
	 */
	EmptyElement(unsigned width, unsigned height);

	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override {}

private:
	unsigned _width;
	unsigned _height;

};

}

#endif
