/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VIGNETTEELEMENT_H
#define RHEELENGINE_VIGNETTEELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API VignetteElement : public Element {

public:
	bool IsOpaque() override;

	void Draw(float time, float dt) const override;

};

}

#endif
