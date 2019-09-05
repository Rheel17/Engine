#include "ShadowMapDirectional.h"

#include "ModelRenderer.h"
#include "SceneRenderManager.h"
#include "../DirectionalLight.h"

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager *manager, LightPtr light) :
		ShadowMap(manager, light) {

	_csm_split = { 10, 10, 30, 50 };
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
		GLFramebuffer buffer(2048, 2048);
		buffer.AddTexture(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
		buffer.Create();

		_shadow_buffers.emplace_back(buffer);
	}

	for (unsigned i = 0; i <= _csm_count; i++) {
		_csm_borders[i] /= accumulator;
	}

	GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, 0);
}

ShadowMapDirectional::~ShadowMapDirectional() {}

void ShadowMapDirectional::Update(CameraPtr camera, unsigned width, unsigned height) {
	GL::PushState();

	// set the lightspace matrices
	 _CalculateViewProjectionMatrices(camera, width, height);

	GLShaderProgram& modelShader = ModelRenderer::GetOpaqueShader();

	for (unsigned i = 0; i < _csm_count; i++) {
		modelShader["lightspaceMatrix"] = _light_matrices[i];

		// write the scene to the framebuffer.
		_shadow_buffers[i].Bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		for (const auto& pair : Manager()->RenderMap()) {
			pair.second.RenderObjects();
		}
	}

	GL::PopState();
}

std::vector<GLTexture2D> ShadowMapDirectional::Textures() const {
	std::vector<GLTexture2D> textures;
	textures.reserve(_csm_count);

	for (const auto& shadowBuffer : _shadow_buffers) {
		textures.emplace_back(shadowBuffer.Textures()[0]);
	}

	return textures;
}

const std::vector<mat4>& ShadowMapDirectional::LightMatrices() const {
	return _light_matrices;
}

void ShadowMapDirectional::_CalculateViewProjectionMatrices(CameraPtr camera, unsigned width, unsigned height) {
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
				_csm_borders[i]     * GetLight()->ShadowDistance(),
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

}
