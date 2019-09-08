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
	const auto& result = _scene_renderer->ResultBuffer();

	GL::ClearFramebufferBinding(GL::FramebufferTarget::DRAW);
	result.BindForReading();

	glBlitFramebuffer(
			0, 0, result.Width(), result.Height(),
			bounds.x, bounds.y, bounds.x + bounds.width, bounds.y + bounds.height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);

	GL::ClearFramebufferBinding(GL::FramebufferTarget::READ);
}

void SceneElement::OnFocusGained() {
	RootContainer()->ParentUI()->GrabMouse(this);
}

void SceneElement::OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_OnKeyPress(key, scancode, mods);
		}
	}
}

void SceneElement::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_OnKeyRelease(key, scancode, mods);
		}
	}
}

void SceneElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_OnMouseButtonPress(button, mods);
		}
	}
}

void SceneElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_OnMouseButtonRelease(button, mods);
		}
	}
}

void SceneElement::OnMouseMove(const vec2& position) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
			inputScript->_OnMouseMove(position);
		}
	}
}

void SceneElement::OnMouseScroll(const vec2& scrollComponents) {
	if (!_scene || !HasFocus()) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (auto inputScript = dynamic_cast<InputScript *>(script.get())) {
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

	_scene_renderer = Engine::GetSceneRenderManager(_scene).CreateSceneRenderer(_camera_name, bounds.width, bounds.height);
}

}
