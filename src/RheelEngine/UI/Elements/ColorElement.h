/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_COLORELEMENT_H
#define RHEELENGINE_COLORELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API ColorElement : public Element {

public:
	/**
	 * Create a color component with a certain color.
	 */
	explicit ColorElement(Color color);

	/**
	 * Create a color component of a certain width and height, and with a
	 * certain color
	 */
	ColorElement(unsigned width, unsigned height, Color color);

	void DoDraw(float time, float dt) const override;

private:
	Color _color;

};

}

#endif
