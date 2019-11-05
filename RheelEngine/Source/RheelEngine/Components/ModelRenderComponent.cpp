/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"

namespace rheel {

ModelRenderComponent::ModelRenderComponent(ModelResource& model, Material material) :
		_model(model), _material(material) {}

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
	if (_material.Type() == Material::Textured) {
		_object_data = Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).AddTexturedObject(_material);
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.GetColor());
	} else {
		_object_data = Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).AddObject();
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.GetColor());
	}
}

void ModelRenderComponent::Render() {
	_object_data.SetMatrix(CalculateAbsoluteTransformationMatrix());
}

void ModelRenderComponent::Deactivate() {
	if (_material.Type() == Material::Textured) {
		Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).RemoveTexturedObject(_material, std::move(_object_data));
	} else {
		Engine::GetSceneRenderManager(GetParent()->scene).GetModelRenderer(_model).RemoveObject(std::move(_object_data));
	}
}

}
