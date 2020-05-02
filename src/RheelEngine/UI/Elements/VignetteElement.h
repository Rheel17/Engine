/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_VIGNETTEELEMENT_H
#define RHEELENGINE_VIGNETTEELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API VignetteElement : public Element {
	struct RE_API vignette_shader {
		vignette_shader();
		Shader shader;
	};
	pseudo_static_pointer<vignette_shader> _shader;

public:
	VignetteElement(Color color, float innerRadius, float outerRadius, float curvature = 1.0f);

public:
	bool IsOpaque() override;

	void DoDraw(float time, float dt) const override;

private:
	Color _color;
	float _inner_radius;
	float _outer_radius;
	float _curvature;

};

}

#endif
