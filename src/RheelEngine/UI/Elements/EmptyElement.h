/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
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

	void Draw(float time, float dt) const override {}

};

}

#endif
