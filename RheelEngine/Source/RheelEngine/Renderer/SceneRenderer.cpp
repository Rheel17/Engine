///*
// * Copyright © 2019 Levi van Rheenen. All rights reserved.
// */
//#include "SceneRenderer.h"
//
//namespace rheel {
//
//SceneRenderer::SceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height, unsigned sampleCount, bool depthComponent) :
//		_manager(manager), _camera_name(std::move(cameraName)),
//		_width(width), _height(height),
//		_result_buffer(width, height, sampleCount) {
//
//	_result_buffer.AddTexture(GL_RGB8, GL_RGB);
//
//	if (depthComponent) {
//		_result_buffer.AddRenderbuffer(GL_DEPTH_COMPONENT32, GL_DEPTH_ATTACHMENT);
//	}
//
//	_result_buffer.Create();
//}
//
//SceneRenderer::~SceneRenderer() {}
//
//void SceneRenderer::SetSize(unsigned width, unsigned height) {
//	if (_width == width && _height == height) {
//		return;
//	}
//
//	_width = width;
//	_height = height;
//
//	_result_buffer = _result_buffer.ResizedCopy(width, height);
//	Resize(width, height);
//}
//
//const GLFramebuffer& SceneRenderer::ResultBuffer() const {
//	return _result_buffer;
//}
//
//void SceneRenderer::_RenderShadowMaps() {
//	Camera *camera = GetCamera();
//	if (!camera || !_manager->ShouldDrawShadows()) {
//		return;
//	}
//
//	_CorrectShadowMapList();
//
//	for (auto& iter : _shadow_maps) {
//		iter.second->Update(*camera, _width, _height);
//	}
//}
//
//SceneRenderManager *SceneRenderer::Manager() const {
//	return _manager;
//}
//
//Camera *SceneRenderer::GetCamera() const {
//	return _manager->GetScene()->GetCamera(_camera_name);
//}
//
//unsigned SceneRenderer::Width() const {
//	return _width;
//}
//
//unsigned SceneRenderer::Height() const {
//	return _height;
//}
//
//const std::unordered_map<std::string, std::unique_ptr<ShadowMap>>& SceneRenderer::ShadowMaps() const {
//	return _shadow_maps;
//}
//
//void SceneRenderer::_CorrectShadowMapList() {
//	std::unordered_set<std::string> lightNames;
//	for (const auto& pair : _shadow_maps) {
//		lightNames.insert(pair.first);
//	}
//
//	for (const std::string& lightName : _manager->GetScene()->Lights()) {
//		Light& light = *(_manager->GetScene()->GetLight(lightName));
//
//		if (!light.CastsShadows()) {
//			continue;
//		}
//
//		lightNames.erase(lightName);
//
//		if (_shadow_maps.find(lightName) == _shadow_maps.end()) {
//			_shadow_maps.insert({ lightName, _manager->CreateShadowMap(lightName) });
//		}
//	}
//
//	for (const std::string& lightName : lightNames) {
//		_shadow_maps.erase(lightName);
//	}
//}
//
//}
