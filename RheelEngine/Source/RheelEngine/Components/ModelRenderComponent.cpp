///*
// * Copyright © 2019 Levi van Rheenen. All rights reserved.
// */
//#include "ModelRenderComponent.h"
//
//#include <iostream>
//
//#include "../Engine.h"
//
//namespace rheel {
//
//void ModelRenderComponent::SetModel(ModelResource& model) {
//	if (_object_data) {
//		throw std::runtime_error("Model cannot be set after initialization");
//	}
//
//	_model = &model;
//}
//
//void ModelRenderComponent::SetMaterial(Material material) {
//	bool wasTextured = _material.Type() == Material::Textured;
//	bool isTextured = material.Type() == Material::Textured;
//
//	_material = std::move(material);
//
//	if (_object_data) {
//		if (!wasTextured && !isTextured) {
//			_object_data.SetMaterialVector(_material.MaterialVector());
//			_object_data.SetMaterialColor(_material.GetColor());
//		} else {
//			OnRemove();
//			OnAdd();
//		}
//	}
//}
//
//const Material& ModelRenderComponent::GetMaterial() const {
//	return _material;
//}
//
//void ModelRenderComponent::OnAdd() {
////	if (_material.Type() == Material::Textured) {
////		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).AddTexturedObject(_material);
////		_object_data.SetMaterialVector(_material.MaterialVector());
////		_object_data.SetMaterialColor(_material.GetColor());
////	} else {
////		_object_data = Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).AddObject();
////		_object_data.SetMaterialVector(_material.MaterialVector());
////		_object_data.SetMaterialColor(_material.GetColor());
////	}
//}
//
//void ModelRenderComponent::OnRender() {
////	mat4 parentMatrix = glm::translate(glm::identity<mat4>(), Parent()->Position()) * glm::mat4_cast(Parent()->Rotation());
////	mat4 componentMatrix = glm::translate(glm::identity<mat4>(), _translation) * glm::scale(glm::mat4_cast(_rotation), _scale);
////
////	_object_data.SetTransform(parentMatrix * componentMatrix);
//}
//
//void ModelRenderComponent::OnRemove() {
////	if (_material.Type() == Material::Textured) {
////		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).RemoveTexturedObject(_material, std::move(_object_data));
////	} else {
////		Engine::GetSceneRenderManager(Parent()->ParentScene()).GetModelRenderer(*_model).RemoveObject(std::move(_object_data));
////	}
//}
//
//}
