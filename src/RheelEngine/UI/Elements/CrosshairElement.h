/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_CROSSHAIRELEMENT_H
#define RHEELENGINE_CROSSHAIRELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API CrosshairElement : public Element {

public:
	CrosshairElement(unsigned size);

	void DoDraw(float time, float dt) const override;

	bool IsOpaque() override;

private:
	unsigned _size;

};

}

#endif
