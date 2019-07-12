#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"
#include "../Object.h"

namespace rheel {

void ModelRenderComponent::SetModel(ModelPtr model) {
	if (_object_data != nullptr) {
		throw std::runtime_error("Model cannot be set after initialization");
	}

	_model = model;
}

void ModelRenderComponent::SetMaterial(Material material) {
	_material = std::move(material);

	if (_object_data != nullptr) {
		_object_data->SetMaterialVector(_material.MaterialVector());
		_object_data->SetMaterialColor(_material.MaterialColor());
	}
}

void ModelRenderComponent::OnAdd() {
	_object_data = Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(_model).AddObject();
	_object_data->SetMaterialVector(_material.MaterialVector());
	_object_data->SetMaterialColor(_material.MaterialColor());
}

void ModelRenderComponent::OnUpdateRenderers() {
	_object_data->SetTransform(Parent().Position(), Parent().Rotation(), Parent().Scale());
}

void ModelRenderComponent::OnRemove() {
	Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(_model).RemoveObject(_object_data);
}

}
