/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneRenderer.h"

namespace rheel {

SceneRenderer::SceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height, unsigned sampleCount, bool depthComponent) :
		_manager(manager), _camera_name(std::move(cameraName)),
		_width(width), _height(height),
		_result_buffer(width, height, sampleCount) {

	_result_buffer.AddTexture(GL_RGB8, GL_RGB);

	if (depthComponent) {
		_result_buffer.AddRenderbuffer(GL_DEPTH_COMPONENT32, GL_DEPTH_ATTACHMENT);
	}

	_result_buffer.Create();
}

void SceneRenderer::SetSize(unsigned width, unsigned height) {
	if (_width == width && _height == height) {
		return;
	}

	_width = width;
	_height = height;

	_result_buffer = _result_buffer.ResizedCopy(width, height);
	Resize(width, height);
}

const GLFramebuffer& SceneRenderer::ResultBuffer() const {
	return _result_buffer;
}

void SceneRenderer::_RenderShadowMaps() {
	Camera *camera = GetCamera();
	if (!camera || !_manager->ShouldDrawShadows()) {
		return;
	}

	_CorrectShadowMapList();

	for (auto& iter : _shadow_maps) {
		iter.second->Update(camera, _width, _height);
	}
}

void SceneRenderer::_RenderSkybox(unsigned width, unsigned height) {
	_manager->GetSkyboxRenderer().Render(GetCamera(), width, height);
}

SceneRenderManager *SceneRenderer::GetManager() const {
	return _manager;
}

Camera *SceneRenderer::GetCamera() const {
	Camera *camera = _manager->GetScene()->GetCamera(_camera_name);

	if (camera == nullptr) {
		Log::Error() << "Camera '" << _camera_name << "' not found" << std::endl;
	}

	return camera;
}

unsigned SceneRenderer::Width() const {
	return _width;
}

unsigned SceneRenderer::Height() const {
	return _height;
}

const std::unordered_map<Light *, std::unique_ptr<ShadowMap>>& SceneRenderer::ShadowMaps() const {
	return _shadow_maps;
}

void SceneRenderer::_CorrectShadowMapList() {
	std::unordered_set<Light *> lights;
	for (const auto& pair : _shadow_maps) {
		lights.insert(pair.first);
	}

	for (Light* light : _manager->GetScene()->GetLights()) {
		if (!light->CastsShadows()) {
			continue;
		}

		lights.erase(light);

		if (_shadow_maps.find(light) == _shadow_maps.end()) {
			_shadow_maps.insert({ light, _manager->CreateShadowMap(light) });
		}
	}

	for (Light *light : lights) {
		_shadow_maps.erase(light);
	}
}

}
