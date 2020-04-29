/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ShadowMapDirectional.h"

#include "../Engine.h"
#include "../Components/DirectionalLight.h"
#include "OpenGL/Context.h"

namespace rheel {

std::unique_ptr<gl::Texture2D> ShadowMapDirectional::_empty_shadow_map;

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager* manager, Light* light) :
		ShadowMap(manager, light) {

	unsigned textureSize = 1024;

	switch (Engine::GetDisplayConfiguration().shadow_quality) {
		case DisplayConfiguration::SHADOW_OFF:
			abort();
		case DisplayConfiguration::SHADOW_LOW:
			textureSize = 1024;
			_bias = 0.005f;
			_csm_split = { 25, 75 };
			break;
		case DisplayConfiguration::SHADOW_MEDIUM:
			textureSize = 1024;
			_bias = 0.005f;
			_csm_split = { 10, 30, 60 };
			break;
		case DisplayConfiguration::SHADOW_HIGH:
			textureSize = 2048;
			_bias = 0.001f;
			_csm_split = { 10, 10, 30, 50 };
			break;
	}

	_csm_count = _csm_split.size();

	_shadow_buffers.reserve(_csm_count);
	_light_matrices.reserve(_csm_count);
	_csm_borders.reserve(_csm_count + 1);
	_csm_borders.push_back(0);

	unsigned accumulator = 0;

	for (unsigned i = 0; i < _csm_count; i++) {
		_csm_borders[i + 1] = accumulator + _csm_split[i];
		accumulator += _csm_split[i];

		// initialize the buffer
		gl::Framebuffer buffer(textureSize, textureSize);
		buffer.AttachTexture(gl::InternalFormat::DEPTH_COMPONENT_32F, gl::Format::DEPTH_COMPONENT, gl::Framebuffer::Attachment::DEPTH);

		// set the texture paramters
		gl::Texture2D& texture = buffer.GetTextureAttachment(gl::Framebuffer::Attachment::DEPTH);
		texture.SetCompareMode(gl::Texture::CompareMode::COMPARE_REF_TO_TEXTURE);
		texture.SetCompareFunction(gl::CompareFunction::LEQUAL);

		_shadow_buffers.emplace_back(std::move(buffer));
	}

	for (unsigned i = 0; i <= _csm_count; i++) {
		_csm_borders[i] /= accumulator;
	}
}

ShadowMapDirectional::~ShadowMapDirectional() = default;

void ShadowMapDirectional::Update(Camera* camera, unsigned width, unsigned height) {
	gl::Context::Current().Push();

	// set the lightspace matrices
	CalculateViewProjectionMatrices_(camera, width, height);

	gl::Program& modelShader = ModelRenderer::GetOpaqueShader();

	for (unsigned i = 0; i < _csm_count; i++) {
		modelShader["lightspaceMatrix"] = _light_matrices[i];

		// write the scene to the framebuffer.
		_shadow_buffers[i].Clear(gl::Framebuffer::BitField::DEPTH);

		for (const auto&[model, renderer] : GetManager()->RenderMap()) {
			renderer.RenderObjects();
		}

		for (const auto&[model, renderer] : GetManager()->CustomShaderRenderMap()) {
			renderer.RenderToShadowMap();
		}
	}

	gl::Context::Current().Pop();
}

std::vector<std::reference_wrapper<const gl::Texture2D>> ShadowMapDirectional::Textures() const {
	std::vector<std::reference_wrapper<const gl::Texture2D>> textures;
	textures.reserve(_csm_count);

	for (const auto& shadowBuffer : _shadow_buffers) {
		textures.push_back(std::ref(shadowBuffer.GetTextureAttachment(gl::Framebuffer::Attachment::DEPTH)));
	}

	return textures;
}

const std::vector<mat4>& ShadowMapDirectional::LightMatrices() const {
	return _light_matrices;
}

float ShadowMapDirectional::Bias() const {
	return _bias;
}

void ShadowMapDirectional::CalculateViewProjectionMatrices_(Camera* camera, unsigned width, unsigned height) {
	// calculate the light coordinate system axis
	vec3 zplus = -GetLight<DirectionalLight>()->Direction();
	vec3 xplus;

	if (zplus.x == 0 && zplus.z == 0) {
		xplus = vec3(1, 0, 0);
	} else {
		xplus = glm::normalize(vec3(zplus.z, 0, -zplus.x));
	}

	vec3 yplus = glm::cross(zplus, xplus);

	mat3 lightMatrix = glm::transpose(mat3(xplus, yplus, zplus));
	mat3 lightMatrixInv = glm::inverse(lightMatrix);

	for (unsigned i = 0; i < _csm_count; i++) {
		// calculate the AABB of the camera frustum in light space
		auto corners = camera->ViewspaceCorners(
				width, height,
				_csm_borders[i] * GetLight()->ShadowDistance(),
				_csm_borders[i + 1] * GetLight()->ShadowDistance());

		float min = std::numeric_limits<float>::lowest();
		float max = std::numeric_limits<float>::max();
		float xMin = max, xMax = min, yMin = max, yMax = min, zMin = max, zMax = min;

		for (vec3 corner : corners) {
			vec3 c = lightMatrix * corner;

			xMin = std::min(xMin, c.x);
			xMax = std::max(xMax, c.x);

			yMin = std::min(yMin, c.y);
			yMax = std::max(yMax, c.y);

			zMin = std::min(zMin, c.z);
			zMax = std::max(zMax, c.z);
		}

		// calculate the light view and projection matrices
		vec3 aabbHalfDim = vec3(xMax - xMin, yMax - yMin, zMax - zMin) / 2.0f;
		aabbHalfDim.x = std::max(aabbHalfDim.x, aabbHalfDim.y);
		aabbHalfDim.y = aabbHalfDim.x;
		aabbHalfDim.z += 200;

		vec3 center = vec3(xMax + xMin, yMax + yMin, zMax + zMin) / 2.0f;
		vec3 centerWorldSpace = lightMatrixInv * center;

		mat4 viewMatrix = glm::translate(mat4(lightMatrix), -centerWorldSpace);
		mat4 projectionMatrix = glm::ortho(
				-aabbHalfDim.x, aabbHalfDim.x,
				-aabbHalfDim.y, aabbHalfDim.y,
				-aabbHalfDim.z, aabbHalfDim.z);

		_light_matrices[i] = projectionMatrix * viewMatrix;
	}
}

const gl::Texture2D& ShadowMapDirectional::EmptyShadowMap() {
	if (!_empty_shadow_map) {
		_empty_shadow_map = std::make_unique<gl::Texture2D>();
		_empty_shadow_map->SetEmpty(gl::InternalFormat::DEPTH_COMPONENT_32F, 1, 1, gl::Format::DEPTH_COMPONENT);
		_empty_shadow_map->SetCompareMode(gl::Texture::CompareMode::COMPARE_REF_TO_TEXTURE);
		_empty_shadow_map->SetCompareFunction(gl::CompareFunction::LEQUAL);
	}

	return *_empty_shadow_map;
}

}
