#include "SceneElement.h"

#include "../../Engine.h"

namespace rheel {

SceneElement::SceneElement(std::string cameraName) :
		_use_active_scene(true), _scene(nullptr), _camera_name(cameraName) {

	SetFocusable(true);
}

SceneElement::SceneElement(Scene *scene, std::string cameraName) :
		_use_active_scene(false), _scene(scene), _camera_name(cameraName) {

	SetFocusable(true);
}

void SceneElement::Draw() const {
	const Bounds& bounds = GetBounds();
	_InitializeRenderer(bounds);

	if (!_scene_renderer) {
		return;
	}

	_scene_renderer->Render();
	_DrawTexturedQuad(bounds, _scene_renderer->OutputTexture());
}

void SceneElement::OnFocusGained() {
	RootContainer()->ParentUI()->GrabMouse(this);
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
