/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneElement.h"

#include "../../Engine.h"
#include "../../Renderer/SceneRenderer.h"
#include "../../Renderer/ShadowMapDirectional.h"
#include "../../Component.h"

namespace rheel {

SceneElement::SceneElement(std::string cameraName) :
		_use_active_scene(true), _scene(nullptr), _camera_name(cameraName),
		_post_processing_stack(std::make_shared<PostProcessingStack>()) {

	SetFocusable(true);
}

SceneElement::SceneElement(Scene *scene, std::string cameraName) :
		_use_active_scene(false), _scene(scene), _camera_name(cameraName),
		_post_processing_stack(std::make_shared<PostProcessingStack>()) {

	SetFocusable(true);
}

void SceneElement::SetGrabOnFocus(bool grabOnFocus) {
	_grab_on_focus = grabOnFocus;
}

bool SceneElement::GetGrabOnFocus() const {
	return _grab_on_focus;
}

PostProcessingStack& SceneElement::GetPostProcessingStack() {
	return *_post_processing_stack;
}

void SceneElement::Draw(float dt) const {
	const Bounds& bounds = GetBounds();
	_InitializeRenderer(bounds);

	if (!_scene_renderer) {
		return;
	}

	_scene_renderer->Render(dt);
	_post_processing_stack->Render(_scene_renderer->ResultBuffer(),
			{ bounds.x, bounds.y }, { bounds.width, bounds.height });
}

void SceneElement::OnFocusGained() {
	if (_grab_on_focus) {
		RootContainer()->ParentUI()->GrabMouse(this);
	}
}

void SceneElement::OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnKeyPress(key, scancode, mods);
	}
}

void SceneElement::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnKeyRelease(key, scancode, mods);
	}
}

void SceneElement::OnCharacterInput(Input::Unicode character) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnCharacterInput(character);
	}
}


void SceneElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseButtonPress(button, mods);
	}
}

void SceneElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseButtonRelease(button, mods);
	}
}

void SceneElement::OnMouseMove(const vec2& position) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseMove(position);
	}
}

void SceneElement::OnMouseJump(const vec2& position) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseJump(position);
	}
}

void SceneElement::OnMouseDrag(const vec2& origin, const vec2& position) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseDrag(origin, position);
	}
}

void SceneElement::OnMouseScroll(const vec2& scrollComponents) {
	if (!_scene) {
		return;
	}

	for (auto inputComponent : _scene->GetInputComponents()) {
		inputComponent->_source = this;
		inputComponent->_OnMouseScroll(scrollComponents);
	}
}

void SceneElement::_InitializeRenderer(const Bounds& bounds) const {
	if (_scene_renderer && (!_use_active_scene || _scene == Engine::GetActiveScene())) {
		_scene_renderer->SetSize(bounds.width, bounds.height);
		return;
	}

	_scene_renderer = nullptr;

	if (_use_active_scene) {
		_scene = Engine::GetActiveScene();
	}

	if (_scene == nullptr) {
		return;
	}

	auto renderer = Engine::GetSceneRenderManager(_scene).CreateSceneRenderer(_camera_name, bounds.width, bounds.height);
	SceneRenderer *ptr = renderer.release();
	_scene_renderer = std::shared_ptr<SceneRenderer>(ptr);
}

}
