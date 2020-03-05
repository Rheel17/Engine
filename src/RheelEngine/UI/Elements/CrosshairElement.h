/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef CROSSHAIRELEMENT_H_
#define CROSSHAIRELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API CrosshairElement : public Element {

public:
	CrosshairElement(unsigned size);

	void Draw(float time, float dt) const override;

	bool IsOpaque() override;

private:
	unsigned _size;

};

}

#endif
