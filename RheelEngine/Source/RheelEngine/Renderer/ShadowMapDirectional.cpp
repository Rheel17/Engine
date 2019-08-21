#include "ShadowMapDirectional.h"

#include <array>

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager *manager, Light *light) :
		ShadowMap(manager, light) {

	_shadow_buffer = std::make_shared<GLFramebuffer>(1024, 1024);
	_shadow_buffer->AddTexture(GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_shadow_buffer->Create();
}

ShadowMapDirectional::~ShadowMapDirectional() {}

void ShadowMapDirectional::Update(const mat4& cameraMatrix, const mat4& cameraMatrixInv) {
	mat4 matrix = _CalculateViewProjectionMatrix(cameraMatrixInv);
}

const GLTexture2D& ShadowMapDirectional::Texture() const {
	return _shadow_buffer->Textures()[0];
}

mat4 ShadowMapDirectional::_CalculateViewProjectionMatrix(const mat4& cameraMatrixInv) const {
	// calculate the light coordinate system axis
	vec3 forward = GetLight()->Direction();
	vec3 right = glm::normalize(vec3(-forward.z, 0, forward.x));
	vec3 up = glm::cross(right, forward);
	mat4 lightMatrix = mat4(mat3(forward, right, up));
	mat4 lightMatrixInv = glm::inverse(lightMatrix);

	// get the coordinates of the corners of the camera frustum in light space
	mat4 lightSpaceTransformation = lightMatrix * cameraMatrixInv;
	std::array<vec4, 8> coordinates = {{
			lightSpaceTransformation * vec4(-1, -1, -1, 0),
			lightSpaceTransformation * vec4(-1, -1,  1, 0),
			lightSpaceTransformation * vec4(-1,  1, -1, 0),
			lightSpaceTransformation * vec4(-1,  1,  1, 0),
			lightSpaceTransformation * vec4( 1, -1, -1, 0),
			lightSpaceTransformation * vec4( 1, -1,  1, 0),
			lightSpaceTransformation * vec4( 1,  1, -1, 0),
			lightSpaceTransformation * vec4( 1,  1,  1, 0),
	}};

	// calculate the AABB of the camera frustum in light space
	float xMin, xMax, yMin, yMax, zMin, zMax;
	for (vec4 c : coordinates) {
		xMin = std::min(xMin, c.x);
		xMax = std::min(xMax, c.x);

		yMin = std::min(yMin, c.y);
		yMax = std::min(yMax, c.y);

		zMin = std::min(zMin, c.z);
		zMax = std::min(zMax, c.z);
	}

	// calculate the light view and projection matrices
	vec3 aabbHalfDim = vec3(xMax - xMin, yMax - yMin, zMax - xMin) / 2.0f;
	vec3 center = vec3(xMax + xMin, yMax + yMin, zMax + zMin) / 2.0f;
	vec4 centerWorldSpace = lightMatrixInv * vec4(center, 1.0f);

	mat4 viewMatrix = glm::translate(lightMatrix, vec3(centerWorldSpace));
	mat4 projectionMatrix = glm::ortho(
			-aabbHalfDim.x, aabbHalfDim.x,
			-aabbHalfDim.y, aabbHalfDim.y,
			-aabbHalfDim.z, aabbHalfDim.z);

	return projectionMatrix * viewMatrix;
}

}
