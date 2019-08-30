#include "ShadowMapDirectional.h"

#include <array>

#include "ModelRenderer.h"
#include "SceneRenderManager.h"
#include "../DirectionalLight.h"

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager *manager, LightPtr light) :
		ShadowMap(manager, light) {

	_shadow_buffer = std::make_shared<GLFramebuffer>(4096, 4096);
	_shadow_buffer->AddTexture(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_shadow_buffer->Create();

	GLTexture2D texture = _shadow_buffer->Textures()[0];
	texture.Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, 0);
}

ShadowMapDirectional::~ShadowMapDirectional() {}

void ShadowMapDirectional::Update(CameraPtr camera, unsigned width, unsigned height) {
	// enable the shadow buffer
	GL::PushState();

	// set the lightspace matrix
	_light_matrix = _CalculateViewProjectionMatrix(camera, width, height);

	GLShaderProgram& modelShader = ModelRenderer::GetOpaqueShader();
	modelShader["lightspaceMatrix"] = _light_matrix;

	// write the scene to the framebuffer.
	_shadow_buffer->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	for (const auto& pair : Manager()->RenderMap()) {
		pair.second.RenderObjects();
	}

	GL::PopState();
}

const GLTexture2D& ShadowMapDirectional::Texture() const {
	return _shadow_buffer->Textures()[0];
}

const mat4& ShadowMapDirectional::LightMatrix() const {
	return _light_matrix;
}

mat4 ShadowMapDirectional::_CalculateViewProjectionMatrix(CameraPtr camera, unsigned width, unsigned height) const {
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

	// calculate the AABB of the camera frustum in light space
	auto corners = camera->ViewspaceCorners(width, height, 0.0f, GetLight()->ShadowDistance());

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

	return projectionMatrix * viewMatrix;
}

}
