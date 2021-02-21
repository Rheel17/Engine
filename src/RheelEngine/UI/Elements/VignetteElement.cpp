/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VignetteElement.h"

#include "../../EngineResources.h"

namespace rheel {

VignetteElement::vignette_shader::vignette_shader() :
		shader(EngineResources::AsString("Shaders_vignette_glsl")) {}

VignetteElement::VignetteElement(Color color, float inner_radius, float outer_radius, float curvature) :
		_color(color),
		_inner_radius(inner_radius),
		_outer_radius(outer_radius),
		_curvature(curvature) {}

bool VignetteElement::IsOpaque() {
	return false;
}

void VignetteElement::DoDraw(float time, float dt) const {
	const auto& shader_program = GetCustomShader(_shader->shader);
	shader_program["parameters"] = vec3{ _inner_radius, _outer_radius, _curvature };
	shader_program["color"] = static_cast<vec4>(_color);

	DrawShaderedQuad(GetBounds(), _shader->shader);
}

}