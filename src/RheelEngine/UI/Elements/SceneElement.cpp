/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneElement.h"

#include "../../Renderer/SceneRenderer.h"
#include "../UI.h"

namespace rheel {

SceneElement::SceneElement(ConstEntityId camera_entity) :
		_use_active_scene(true),
		_scene(nullptr),
		_camera_entity(camera_entity),
		_post_processing_stack(std::make_shared<PostProcessingStack>()) {

	SetFocusable(true);
}

SceneElement::SceneElement(Scene* scene, ConstEntityId camera_entity) :
		_use_active_scene(false),
		_scene(scene),
		_camera_entity(camera_entity),
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

void SceneElement::DoDraw(float time, float dt) const {
	gl::Context::Current().Enable(gl::Capability::DEPTH_TEST);

	const Bounds& bounds = GetBounds();
	InitializeRenderer_(bounds);

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

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->_pressed_keys.insert(key);
		input_component->_pressed_scancodes.insert(scancode);
		input_component->OnKeyPress(key, scancode, mods);
	}
}

void SceneElement::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->_pressed_keys.erase(key);
		input_component->_pressed_scancodes.erase(scancode);
		input_component->OnKeyRelease(key, scancode, mods);
	}
}

void SceneElement::OnCharacterInput(Input::Unicode character) {
	if (!_scene) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->OnCharacterInput(character);
	}
}

void SceneElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->OnMouseButtonPress(button, mods);
	}
}

void SceneElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->OnMouseButtonRelease(button, mods);
	}
}

void SceneElement::OnMouseMove(const vec2& position) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		if (input_component->_got_mouse) {
			input_component->_mouse_delta = position - input_component->_mouse_position;
		}

		input_component->_mouse_position = position;
		input_component->_got_mouse = true;
		input_component->OnMouseMove(position);
	}
}

void SceneElement::OnMouseJump(const vec2& position) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->_mouse_delta = vec2{};
		input_component->_mouse_position = position;
		input_component->_got_mouse = true;
		input_component->OnMouseMove(position);
	}
}

void SceneElement::OnMouseDrag(const vec2& origin, const vec2& position) {
	if (!_scene) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		if (input_component->_got_mouse) {
			input_component->_mouse_delta = position - input_component->_mouse_position;
		}

		input_component->_mouse_position = position;
		input_component->_got_mouse = true;

		input_component->OnMouseDrag(origin, position);
	}
}

void SceneElement::OnMouseScroll(const vec2& scroll_components) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (auto* input_component : _scene->GetRegistry().GetInputComponents()) {
		input_component->_mouse_scroll = scroll_components;
		input_component->OnMouseMove(scroll_components);
	}
}

void SceneElement::InitializeRenderer_(const Bounds& bounds) const {
	auto& game = RootContainer()->ParentUI()->GetGame();

	if (_scene_renderer && (!_use_active_scene || _scene == game.GetActiveScene())) {
		_scene_renderer->SetSize(bounds.width, bounds.height);
		return;
	}

	_scene_renderer = nullptr;

	if (_use_active_scene) {
		_scene = game.GetActiveScene();
	}

	if (_scene == nullptr) {
		return;
	}

	auto renderer = game.GetRenderer().GetSceneRenderManager(_scene).CreateSceneRenderer(_camera_entity, bounds.width, bounds.height);
	SceneRenderer* ptr = renderer.release();
	_scene_renderer = std::shared_ptr<SceneRenderer>(ptr);
}

}
