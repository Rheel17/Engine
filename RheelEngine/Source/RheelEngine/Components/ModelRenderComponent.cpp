/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"
#include "../Object.h"

namespace rheel {

ModelRenderComponent::ModelRenderComponent(const ModelRenderComponent& component) :
		_model(component._model), _material(component._material), _scale(component._scale) {}

void ModelRenderComponent::SetModel(ModelResource& model) {
	if (_object_data) {
		throw std::runtime_error("Model cannot be set after initialization");
	}

	_model = &model;
}

void ModelRenderComponent::SetMaterial(Material material) {
	bool wasTextured = _material.Type() == Material::Textured;
	bool isTextured = material.Type() == Material::Textured;

	_material = std::move(material);

	if (_object_data) {
		if (!wasTextured && !isTextured) {
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
		} else {
			OnRemove();
			OnAdd();
		}
	}
}

const Material& ModelRenderComponent::GetMaterial() const {
	return _material;
}

void ModelRenderComponent::SetTranslation(const vec3& translation) {
	_translation = translation;
}

const vec3& ModelRenderComponent::Translation() const {
	return _translation;
}

void ModelRenderComponent::SetRotation(const quat& rotation) {
	_rotation = rotation;
}

const quat& ModelRenderComponent::Rotation() const {
	return _rotation;
}

void ModelRenderComponent::SetScale(const vec3& scale) {
	SetScale(scale.x, scale.y, scale.z);
}

void ModelRenderComponent::SetScale(float x, float y, float z) {
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

void ModelRenderComponent::SetScale(float scale) {
	SetScale(scale, scale, scale);
}

const vec3& ModelRenderComponent::Scale() const {
	return _scale;
}

void ModelRenderComponent::OnAdd() {
	if (_material.Type() == Material::Textured) {
		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).AddTexturedObject(_material);
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.GetColor());
	} else {
		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).AddObject();
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.GetColor());
	}
}

void ModelRenderComponent::OnUpdateRenderers() {
	mat4 parentMatrix = glm::translate(glm::identity<mat4>(), Parent()->Position()) * glm::mat4_cast(Parent()->Rotation());
	mat4 componentMatrix = glm::translate(glm::identity<mat4>(), _translation) * glm::scale(glm::mat4_cast(_rotation), _scale);

	_object_data.SetTransform(parentMatrix * componentMatrix);
}

void ModelRenderComponent::OnRemove() {
	if (_material.Type() == Material::Textured) {
		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).RemoveTexturedObject(_material, std::move(_object_data));
	} else {
		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).RemoveObject(std::move(_object_data));
	}
}

}
