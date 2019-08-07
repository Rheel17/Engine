#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"
#include "../Object.h"

namespace rheel {

void ModelRenderComponent::SetModel(ModelPtr model) {
	if (_object_data) {
		throw std::runtime_error("Model cannot be set after initialization");
	}

	_model = model;
}

void ModelRenderComponent::SetMaterial(Material material) {
	bool wasTextured = _material.Type() == Material::Textured;
	bool isTextured = material.Type() == Material::Textured;

	Material oldMaterial = _material;
	_material = std::move(material);

	if (_object_data) {
		if (!wasTextured && !isTextured) {
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.MaterialColor());
		} else {
			OnRemove();
			OnAdd();
		}
	}
}

void ModelRenderComponent::OnAdd() {
	if (_material.Type() == Material::Textured) {
		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(_model).AddTexturedObject(_material);
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.MaterialColor());
	} else {
		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(_model).AddObject();
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.MaterialColor());
	}
}

void ModelRenderComponent::OnUpdateRenderers() {
	_object_data.SetTransform(Parent()->Position(), Parent()->Rotation(), Parent()->Scale());
}

void ModelRenderComponent::OnRemove() {
	if (_material.Type() == Material::Textured) {
		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(_model).RemoveTexturedObject(_material, std::move(_object_data));
	} else {
		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(_model).RemoveObject(std::move(_object_data));
	}
}

}
