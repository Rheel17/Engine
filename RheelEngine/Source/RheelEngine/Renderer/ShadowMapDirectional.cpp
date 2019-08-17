#include "ShadowMapDirectional.h"

namespace rheel {

ShadowMapDirectional::ShadowMapDirectional(SceneRenderManager *manager, Light *light) :
		ShadowMap(manager, light) {}

ShadowMapDirectional::~ShadowMapDirectional() {}

void ShadowMapDirectional::Update(const mat4& cameraMatrix, const mat4& cameraMatrixInv) {
	// calculate the light coordinate system axis
	vec3 forward = GetLight()->Direction();
	vec3 right = glm::normalize(vec3(-forward.z, 0, forward.x));
	vec3 up = glm::cross(right, forward);
}

}
