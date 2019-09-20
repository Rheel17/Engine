#include "../Components/ModelRenderComponent.h"

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
			_object_data.SetMaterialColor(_material.MaterialColor());
		} else {
			OnRemove();
			OnAdd();
		}
	}
}

void ModelRenderComponent::OnAdd() {
	if (_material.Type() == Material::Textured) {
		_object_data = Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(*_model).AddTexturedObject(_material);
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.MaterialColor());
	} else {
		_object_data = Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(*_model).AddObject();
		_object_data.SetMaterialVector(_material.MaterialVector());
		_object_data.SetMaterialColor(_material.MaterialColor());
	}
}

void ModelRenderComponent::OnUpdateRenderers() {
	_object_data.SetTransform(Parent().Position(), Parent().Rotation(), _scale);
}

void ModelRenderComponent::OnRemove() {
	if (_material.Type() == Material::Textured) {
		Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(*_model).RemoveTexturedObject(_material, std::move(_object_data));
	} else {
		Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(*_model).RemoveObject(std::move(_object_data));
	}
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

}
