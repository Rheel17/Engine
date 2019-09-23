#include "SceneElement.h"

#include "../../Engine.h"
#include "../../Renderer/SceneRenderer.h"
#include "../../Renderer/ShadowMapDirectional.h"
#include "../../Scripts/ScriptInput.h"

namespace rheel {

SceneElement::SceneElement(std::string cameraName) :
		_use_active_scene(true), _scene(nullptr), _camera_name(cameraName) {

	SetFocusable(true);
}

SceneElement::SceneElement(Scene *scene, std::string cameraName) :
		_use_active_scene(false), _scene(scene), _camera_name(cameraName) {

	SetFocusable(true);
}

void SceneElement::SetGrabOnFocus(bool grabOnFocus) {
	_grab_on_focus = grabOnFocus;
}

bool SceneElement::GetGrabOnFocus() const {
	return _grab_on_focus;
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
	if (_grab_on_focus) {
		RootContainer()->ParentUI()->GrabMouse(this);
	}
}

void SceneElement::OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnKeyPress(key, scancode, mods);
		}
	}
}

void SceneElement::OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	if (!_scene) {
		return;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnKeyRelease(key, scancode, mods);
		}
	}
}

bool SceneElement::OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return true;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnMouseButtonPress(button, mods);
		}
	}

	return true;
}

bool SceneElement::OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	if (!_scene) {
		return true;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnMouseButtonRelease(button, mods);
		}
	}

	return true;
}

bool SceneElement::OnMouseMove(const vec2& position) {
	if (!_scene) {
		return true;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnMouseMove(position);
		}
	}

	return true;
}

bool SceneElement::OnMouseScroll(const vec2& scrollComponents) {
	if (!_scene) {
		return true;
	}

	for (const auto& script : _scene->Scripts()) {
		if (script->IsActive()) {
			script->_source_element = this;
			script->_OnMouseScroll(scrollComponents);
		}
	}

	return true;
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
