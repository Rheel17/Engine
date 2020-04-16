/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_EMPTYELEMENT_H
#define RHEELENGINE_EMPTYELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API EmptyElement : public Element {

public:
	/**
	 * Creates an empty element of a certain width and height.
	 */
	EmptyElement(unsigned width, unsigned height);

	void Draw(float time, float dt) const override {}

};

}

#endif
