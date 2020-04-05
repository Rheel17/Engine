/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "VignetteElement.h"

#include "../../EngineResources.h"

namespace rheel {

std::unique_ptr<Shader> VignetteElement::_shader;
bool VignetteElement::_initialized;

VignetteElement::VignetteElement(Color color, float innerRadius, float outerRadius, float curvature) :
		_color(color), _inner_radius(innerRadius), _outer_radius(outerRadius), _curvature(curvature) {}

bool VignetteElement::IsOpaque() {
	return false;
}

void VignetteElement::Draw(float time, float dt) const {
	_Initialize();

	const auto& shaderProgram = _GetCustomShader(*_shader);
	shaderProgram["parameters"] = vec3{ _inner_radius, _outer_radius, _curvature };
	shaderProgram["color"] = _color;

	_DrawShaderedQuad(GetBounds(), *_shader);
}

void VignetteElement::_Initialize() {
	if (_initialized) {
		return;
	}

	_shader = std::make_unique<Shader>(EngineResources::AsString("Shaders_vignette_glsl"));
	_initialized = true;
}

}