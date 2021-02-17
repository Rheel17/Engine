/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "SceneRenderer.h"

#include "../Components/PointLight.h"
#include "../Components/SpotLight.h"
#include "../Components/DirectionalLight.h"

namespace rheel {

SceneRenderer::SceneRenderer(
		SceneRenderManager* manager,
		ConstEntityId camera_entity,
		unsigned width,
		unsigned height,
		unsigned sampleCount,
		bool depthComponent) :
		_manager(manager),
		_camera_entity(camera_entity),
		_width(width),
		_height(height),
		_result_buffer(width, height) {

	if (sampleCount > 1) {
		_result_buffer.AttachTextureMultisample(gl::InternalFormat::RGB8, sampleCount, 0);

		if (depthComponent) {
			_result_buffer.AttachRenderbufferMultisample(gl::InternalFormat::DEPTH_COMPONENT_32F, sampleCount, gl::Framebuffer::Attachment::DEPTH);
		}
	} else {
		_result_buffer.AttachTexture(gl::InternalFormat::RGB8, gl::Format::RGB, 0);

		if (depthComponent) {
			_result_buffer.AttachRenderbuffer(gl::InternalFormat::DEPTH_COMPONENT_32F, gl::Framebuffer::Attachment::DEPTH);
		}
	}

	_result_buffer.SetDrawBuffers({ 0 });
}

void SceneRenderer::SetSize(unsigned width, unsigned height) {
	if (_width == width && _height == height) {
		return;
	}

	_width = width;
	_height = height;

	_result_buffer = gl::Framebuffer(_result_buffer, width, height);
	Resize(width, height);
}

const gl::Framebuffer& SceneRenderer::ResultBuffer() const {
	return _result_buffer;
}

void SceneRenderer::RenderShadowMaps() {
	const Camera* camera = GetCamera();
	if (!camera || !_manager->ShouldDrawShadows()) {
		return;
	}

	CorrectShadowMapList_();

	for (auto& iter : _shadow_maps) {
		iter.second->Update(camera, _width, _height);
	}
}

void SceneRenderer::RenderSkybox(unsigned width, unsigned height) {
	_manager->GetSkyboxRenderer().Render(GetCamera(), width, height);
}

SceneRenderManager* SceneRenderer::GetManager() const {
	return _manager;
}

const Camera* SceneRenderer::GetCamera() const {
	return _manager->GetScene()->GetRegistry().GetEntity(_camera_entity)->GetComponent<Camera>();
}

unsigned SceneRenderer::Width() const {
	return _width;
}

unsigned SceneRenderer::Height() const {
	return _height;
}

const std::map<const Light*, std::unique_ptr<ShadowMap>>& SceneRenderer::ShadowMaps() const {
	return _shadow_maps;
}

void SceneRenderer::CorrectShadowMapList_() {
	std::unordered_set<const Light*> lights;
	for (const auto& pair : _shadow_maps) {
		lights.insert(pair.first);
	}

	auto all_lights = _manager->GetScene()->GetRegistry().GetComponents<PointLight, SpotLight, DirectionalLight>().As<Light>();
	for (const auto& light : all_lights) {
		if (!light.CastsShadows()) {
			return;
		}

		lights.erase(&light);

		if (_shadow_maps.find(&light) == _shadow_maps.end()) {
			_shadow_maps.insert({ &light, _manager->CreateShadowMap(light) });
		}
	}

	for (const Light* light : lights) {
		_shadow_maps.erase(light);
	}
}

}
