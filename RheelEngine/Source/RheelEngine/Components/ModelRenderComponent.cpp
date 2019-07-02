#include "ModelRenderComponent.h"

#include <iostream>

#include "../Engine.h"
#include "../Object.h"

namespace rheel {

void ModelRenderComponent::SetModel(ModelPtr model) {
	_model = model;
}

void ModelRenderComponent::OnAdd() {
	_object_data = Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(_model).AddObject();
}

void ModelRenderComponent::OnUpdateRenderers() {
	_object_data->SetTransform(Parent().Position(), Parent().Rotation(), Parent().Scale());
}

void ModelRenderComponent::OnRemove() {
	Engine::GetSceneRenderManager(Parent().ParentScene()).GetModelRenderer(_model).RemoveObject(_object_data);
}

}
