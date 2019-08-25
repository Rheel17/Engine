#include "ShadowMapDirectional.h"

#include <array>

#include "ModelRenderer.h"
#include "SceneRenderManager.h"

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager *manager, Light *light) :
		ShadowMap(manager, light) {

	_shadow_buffer = std::make_shared<GLFramebuffer>(1024, 1024);
	_shadow_buffer->AddTexture(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_shadow_buffer->Create();
}

ShadowMapDirectional::~ShadowMapDirectional() {}

void ShadowMapDirectional::Update(CameraPtr camera, unsigned width, unsigned height) {
	// enable the shadow buffer
	GL::PushState();
	_shadow_buffer->BindForDrawing();

	// set the lightspace matrix
	GLShaderProgram& opaqueShader = ModelRenderer::GetOpaqueShader();
	mat4 matrix = _CalculateViewProjectionMatrix(camera, width, height);
	opaqueShader["lightspaceMatrix"] = matrix;
	light_matrix = matrix;

	for (const auto& pair : Manager()->RenderMap()) {
		pair.second.RenderObjects();
	}

	GL::PopState();
}

const GLTexture2D& ShadowMapDirectional::Texture() const {
	return _shadow_buffer->Textures()[0];
}

mat4 ShadowMapDirectional::_CalculateViewProjectionMatrix(CameraPtr camera, unsigned width, unsigned height) const {
	// calculate the light coordinate system axis
	vec3 zplus = -GetLight()->Direction();
	vec3 xplus;

	if (zplus.x == 0 && zplus.z == 0) {
		xplus = vec3(1, 0, 0);
	} else {
		xplus = glm::normalize(vec3(zplus.z, 0, -zplus.x));
	}

	vec3 yplus = glm::cross(zplus, xplus);

//	std::cout << "x+: " << xplus << std::endl;
//	std::cout << "y+: " << yplus << std::endl;
//	std::cout << "z+: " << zplus << std::endl << std::endl;

	mat4 lightMatrix = mat4(glm::transpose(mat3(xplus, yplus, zplus)));

	std::cout << lightMatrix << std::endl;

	mat4 lightMatrixInv = glm::inverse(lightMatrix);

	// calculate the AABB of the camera frustum in light space
	auto corners = camera->ViewspaceCorners(width, height);

	float min = std::numeric_limits<float>::min();
	float max = std::numeric_limits<float>::max();
	float xMin = max, xMax = min, yMin = max, yMax = min, zMin = max, zMax = min;

	for (vec3 corner : corners) {
		vec4 c = lightMatrix * vec4(corner, 1.0);

		xMin = std::min(xMin, c.x);
		xMax = std::max(xMax, c.x);

		yMin = std::min(yMin, c.y);
		yMax = std::max(yMax, c.y);

		zMin = std::min(zMin, c.z);
		zMax = std::max(zMax, c.z);
	}

	// calculate the light view and projection matrices
	vec3 aabbHalfDim = vec3(xMax - xMin, yMax - yMin, zMax - xMin) / 2.0f;
	vec3 center = vec3(xMax + xMin, yMax + yMin, zMax + zMin) / 2.0f;
	vec4 centerWorldSpace = lightMatrixInv * vec4(center, 1.0f);

	mat4 viewMatrix = glm::translate(lightMatrix, -vec3(centerWorldSpace));
	mat4 projectionMatrix = glm::ortho(
			-aabbHalfDim.x, aabbHalfDim.x,
			-aabbHalfDim.y, aabbHalfDim.y,
			-aabbHalfDim.z, aabbHalfDim.z);

	return projectionMatrix * viewMatrix;
}

}
