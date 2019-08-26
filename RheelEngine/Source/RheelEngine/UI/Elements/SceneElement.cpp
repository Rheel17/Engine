#include "SceneElement.h"

#include "../../Engine.h"
#include "../../Scripts/InputScript.h"
#include "../../Renderer/ShadowMapDirectional.h"

namespace rheel {

SceneElement::SceneElement(std::string cameraName) :
		_use_active_scene(true), _scene(nullptr), _camera_name(cameraName) {

	SetFocusable(true);
}

SceneElement::SceneElement(Scene *scene, std::string cameraName) :
		_use_active_scene(false), _scene(scene), _camera_name(cameraName) {

	SetFocusable(true);
}

void SceneElement::Draw(float dt) const {
	const Bounds& bounds = GetBounds();
	_InitializeRenderer(bounds);

	if (!_scene_renderer) {
		return;
	}

	_scene_renderer->Render(dt);
	_DrawTexturedQuad(bounds, _scene_renderer->OutputTexture());
}

void SceneElement::OnFocusGained() {
	RootContainer()->ParentUI()->GrabMouse(this);
}

void SceneElement::OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnKeyPress(key, scancode, mods);
		}
	}
}

void SceneElement::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnKeyRelease(key, scancode, mods);
		}
	}
}

void SceneElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnMouseButtonPress(button, mods);
		}
	}
}

void SceneElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnMouseButtonRelease(button, mods);
		}
	}
}

void SceneElement::OnMouseMove(const vec2& position) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnMouseMove(position);
		}
	}
}

void SceneElement::OnMouseScroll(const vec2& scrollComponents) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (const auto inputScript = std::dynamic_pointer_cast<InputScript>(script)) {
			inputScript->_OnMouseScroll(scrollComponents);
		}
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

	_scene_renderer = std::make_shared<SceneRenderer>(Engine::GetSceneRenderManager(_scene).CreateSceneRenderer(_camera_name, bounds.width, bounds.height));
}

}
