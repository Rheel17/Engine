/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ShadowMapDirectional.h"

#include "../Components/DirectionalLight.h"

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager* manager, const Light& light) :
		ShadowMap(manager, light) {

	unsigned texture_size = 1024;

	switch (DisplayConfiguration::Get().shadow_quality) {
		case DisplayConfiguration::SHADOW_OFF:
			abort();
		case DisplayConfiguration::SHADOW_LOW:
			texture_size = 1024;
			_bias = 0.005f;
			_csm_split = { 25, 75 };
			break;
		case DisplayConfiguration::SHADOW_MEDIUM:
			texture_size = 1024;
			_bias = 0.005f;
			_csm_split = { 10, 30, 60 };
			break;
		case DisplayConfiguration::SHADOW_HIGH:
			texture_size = 2048;
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
		gl::Framebuffer buffer(texture_size, texture_size);
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

void ShadowMapDirectional::Update(const Camera* camera, unsigned width, unsigned height) {
	gl::ContextScope cs;

	// set the lightspace matrices
	_calculate_view_projection_matrices(camera, width, height);

	gl::Program& model_shader = GetManager()->GetOpaqueShader();

	for (unsigned i = 0; i < _csm_count; i++) {
		model_shader["lightspaceMatrix"] = _light_matrices[i];

		// write the scene to the framebuffer.
		_shadow_buffers[i].Clear(gl::Framebuffer::BitField::DEPTH);

		for (const auto&[model, renderer] : GetManager()->RenderMap()) {
			renderer.RenderObjects();
		}

		for (const auto&[model, renderer] : GetManager()->CustomShaderRenderMap()) {
			renderer.RenderToShadowMap();
		}
	}
}

std::vector<std::reference_wrapper<const gl::Texture2D>> ShadowMapDirectional::Textures() const {
	std::vector<std::reference_wrapper<const gl::Texture2D>> textures;
	textures.reserve(_csm_count);

	for (const auto& shadow_buffer : _shadow_buffers) {
		textures.push_back(std::ref(shadow_buffer.GetTextureAttachment(gl::Framebuffer::Attachment::DEPTH)));
	}

	return textures;
}

const std::vector<mat4>& ShadowMapDirectional::LightMatrices() const {
	return _light_matrices;
}

float ShadowMapDirectional::Bias() const {
	return _bias;
}

void ShadowMapDirectional::_calculate_view_projection_matrices(const Camera* camera, unsigned width, unsigned height) {
	// calculate the light coordinate system axis
	vec3 zplus = -GetLight<DirectionalLight>().Direction();
	vec3 xplus;

	if (zplus.x == 0 && zplus.z == 0) {
		xplus = vec3(1, 0, 0);
	} else {
		xplus = glm::normalize(vec3(zplus.z, 0, -zplus.x));
	}

	vec3 yplus = glm::cross(zplus, xplus);

	mat3 light_matrix = glm::transpose(mat3(xplus, yplus, zplus));
	mat3 light_matrix_inv = glm::inverse(light_matrix);

	for (unsigned i = 0; i < _csm_count; i++) {
		// calculate the AABB of the camera frustum in light space
		auto corners = camera->ViewspaceCorners(
				width, height,
				_csm_borders[i] * GetLight().ShadowDistance(),
				_csm_borders[i + 1] * GetLight().ShadowDistance());

		float min = std::numeric_limits<float>::lowest();
		float max = std::numeric_limits<float>::max();
		float x_min = max, x_max = min, y_min = max, y_max = min, z_min = max, z_max = min;

		for (vec3 corner : corners) {
			vec3 c = light_matrix * corner;

			x_min = std::min(x_min, c.x);
			x_max = std::max(x_max, c.x);

			y_min = std::min(y_min, c.y);
			y_max = std::max(y_max, c.y);

			z_min = std::min(z_min, c.z);
			z_max = std::max(z_max, c.z);
		}

		// calculate the light view and projection matrices
		vec3 aabb_half_dim = vec3(x_max - x_min, y_max - y_min, z_max - z_min) / 2.0f;
		aabb_half_dim.x = std::max(aabb_half_dim.x, aabb_half_dim.y);
		aabb_half_dim.y = aabb_half_dim.x;
		aabb_half_dim.z += 200;

		vec3 center = vec3(x_max + x_min, y_max + y_min, z_max + z_min) / 2.0f;
		vec3 center_world_space = light_matrix_inv * center;

		mat4 view_matrix = glm::translate(mat4(light_matrix), -center_world_space);
		mat4 projection_matrix = glm::ortho(
				-aabb_half_dim.x, aabb_half_dim.x,
				-aabb_half_dim.y, aabb_half_dim.y,
				-aabb_half_dim.z, aabb_half_dim.z);

		_light_matrices[i] = projection_matrix * view_matrix;
	}
}

}
