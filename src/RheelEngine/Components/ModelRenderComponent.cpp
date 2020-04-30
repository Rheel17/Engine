/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderComponent.h"

namespace rheel {

ModelRenderComponent::ModelRenderComponent(Model model, Material material) :
		_model(std::move(model)),
		_material(material) {}

void ModelRenderComponent::SetMaterial(Material material) {
	bool wasTextured = _material.Type() == Material::TEXTURED;
	bool isTextured = material.Type() == Material::TEXTURED;

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
	auto& sceneRenderManager = GetParent()->scene->GetGame().GetRenderer().GetSceneRenderManager(GetParent()->scene);

	switch (_material.Type()) {
		case Material::COLORED:
			_object_data = sceneRenderManager.GetModelRenderer(_model).AddObject();
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::TEXTURED:
			_object_data = sceneRenderManager.GetModelRenderer(_model).AddTexturedObject(_material);
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::CUSTOM_SHADER:
			_object_data = sceneRenderManager.GetModelRendererForCustomShader(_model, _material.GetCustomShader()).AddObject();
			break;
	}
}

void ModelRenderComponent::Render() {
	_object_data.SetMatrix(CalculateAbsoluteTransformationMatrix());
}

void ModelRenderComponent::Deactivate() {
	auto& sceneRenderManager = GetParent()->scene->GetGame().GetRenderer().GetSceneRenderManager(GetParent()->scene);

	switch (_material.Type()) {
		case Material::COLORED:
			sceneRenderManager.GetModelRenderer(_model).RemoveObject(std::move(_object_data));
			break;
		case Material::TEXTURED:
			sceneRenderManager.GetModelRenderer(_model).RemoveTexturedObject(_material, std::move(_object_data));
			break;
		case Material::CUSTOM_SHADER:
			sceneRenderManager.GetModelRendererForCustomShader(_model, _material.GetCustomShader()).RemoveObject(std::move(_object_data));
			break;
	}
}

}
