#ifndef EMPTYELEMENT_H_
#define EMPTYELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API EmptyElement : public Element {

	__ELEMENT__

public:
	/**
	 * Creates an empty element of a certain width and height.
	 */
	EmptyElement(unsigned width, unsigned height);

	void Draw() const override {}

};

}

#endif
