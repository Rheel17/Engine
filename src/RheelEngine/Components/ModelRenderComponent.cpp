/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"

namespace rheel {

ModelRenderComponent::ModelRenderComponent(Model model, Material material) :
		_model(std::move(model)), _material(material) {}

void ModelRenderComponent::SetMaterial(Material material) {
	bool wasTextured = _material.Type() == Material::Textured;
	bool isTextured = material.Type() == Material::Textured;

	_material = std::move(material);

	if (_object_data) {
		if (!wasTextured && !isTextured) {
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
		} else {
			Deactivate();
			Activate();
		}
	}
}

const Material& ModelRenderComponent::GetMaterial() const {
	return _material;
}

void ModelRenderComponent::Activate() {
	switch (_material.Type()) {
		case Material::Colored:
			_object_data = Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).AddObject();
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::Textured:
			_object_data = Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).AddTexturedObject(_material);
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::CustomShader:
			_object_data = Engine::GetSceneRenderManager(GetParent()->scene).GetModelRendererForCustomShader(_model, _material.GetCustomShader()).AddObject();
			break;
	}
}

void ModelRenderComponent::Render() {
	_object_data.SetMatrix(CalculateAbsoluteTransformationMatrix());
}

void ModelRenderComponent::Deactivate() {
	switch (_material.Type()) {
		case Material::Colored:
			Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).RemoveObject(std::move(_object_data));
			break;
		case Material::Textured:
			Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).RemoveTexturedObject(_material, std::move(_object_data));
			break;
		case Material::CustomShader:
			Engine::GetSceneRenderManager(GetParent()->scene).GetModelRendererForCustomShader(_model, _material.GetCustomShader()).RemoveObject(std::move(_object_data));
			break;
	}
}

}
