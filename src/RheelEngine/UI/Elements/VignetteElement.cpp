/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VignetteElement.h"

#include "../../EngineResources.h"

namespace rheel {

VignetteElement::vignette_shader::vignette_shader() :
		shader(EngineResources::AsString("Shaders_vignette_glsl")) {}

VignetteElement::VignetteElement(Color color, float innerRadius, float outerRadius, float curvature) :
		_color(color),
		_inner_radius(innerRadius),
		_outer_radius(outerRadius),
		_curvature(curvature) {}

bool VignetteElement::IsOpaque() {
	return false;
}

void VignetteElement::DoDraw(float time, float dt) const {
	const auto& shaderProgram = GetCustomShader(_shader->shader);
	shaderProgram["parameters"] = vec3{ _inner_radius, _outer_radius, _curvature };
	shaderProgram["color"] = static_cast<vec4>(_color);

	DrawShaderedQuad(GetBounds(), _shader->shader);
}

}