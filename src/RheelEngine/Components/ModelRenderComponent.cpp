/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderComponent.h"

#include "../Registry/Registry.h"
#include "../Scene.h"

namespace rheel {

ModelRenderComponent::ModelRenderComponent(Model model, Material material) :
		_model(std::move(model)),
		_material(std::move(material)) {}

void ModelRenderComponent::SetMaterial(Material material) {
	bool was_textured = _material.Type() == Material::TEXTURED;
	bool is_textured = material.Type() == Material::TEXTURED;

	_material = std::move(material);

	if (_object_data) {
		if (!was_textured && !is_textured) {
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
		} else {
			OnDeactivate();
			OnActivate();
		}
	}
}

const Material& ModelRenderComponent::GetMaterial() const {
	return _material;
}

void ModelRenderComponent::OnActivate() {
	auto& scene = GetEntity().GetScene();
	auto& scene_render_manager = scene.GetGame().GetRenderer().GetSceneRenderManager(&scene);

	switch (_material.Type()) {
		case Material::COLORED:
			_object_data = scene_render_manager.GetModelRenderer(_model).AddObject();
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::TEXTURED:
			_object_data = scene_render_manager.GetModelRenderer(_model).AddTexturedObject(_material);
			_object_data.SetMaterialVector(_material.MaterialVector());
			_object_data.SetMaterialColor(_material.GetColor());
			break;
		case Material::CUSTOM_SHADER:
			_object_data = scene_render_manager.GetModelRendererForCustomShader(_model, _material.GetCustomShader()).AddObject();
			break;
	}
}

void ModelRenderComponent::OnDeactivate() {
	auto& scene = GetEntity().GetScene();
	auto& scene_render_manager = scene.GetGame().GetRenderer().GetSceneRenderManager(&scene);

	switch (_material.Type()) {
		case Material::COLORED:
			scene_render_manager.GetModelRenderer(_model).RemoveObject(std::move(_object_data));
			break;
		case Material::TEXTURED:
			scene_render_manager.GetModelRenderer(_model).RemoveTexturedObject(_material, std::move(_object_data));
			break;
		case Material::CUSTOM_SHADER:
			scene_render_manager.GetModelRendererForCustomShader(_model, _material.GetCustomShader()).RemoveObject(std::move(_object_data));
			break;
	}
}

void ModelRenderComponent::Update() {
	// TODO: only if matrix changed
	_object_data.SetMatrix(GetEntity().AbsoluteTransform().AsMatrix());
}

}
