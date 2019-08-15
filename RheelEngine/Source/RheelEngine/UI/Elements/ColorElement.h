#ifndef COLORELEMENT_H_
#define COLORELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API ColorElement : public Element {
	friend class UI;

public:
	/**
	 * Create a color component with a certain color.
	 */
	ColorElement(Color color);

	/**
	 * Create a color component of a certain width and height, and with a
	 * certain color
	 */
	ColorElement(unsigned width, unsigned height, Color color);

	void Draw(float dt) const override;

private:
	Color _color;

};

}

#endif
