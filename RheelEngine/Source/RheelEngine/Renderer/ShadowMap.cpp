#include "ShadowMap.h"

#include "SceneRenderManager.h"

#include "ShadowMapDirectional.h"

namespace rheel {

ShadowMap::ShadowMap(SceneRenderManager *manager, Light *light) :
		_manager(manager), _light(light) {}

SceneRenderManager *ShadowMap::Manager() const {
	return _manager;
}

Light *ShadowMap::GetLight() const {
	return _light;
}

std::shared_ptr<ShadowMap> ShadowMap::CreateShadowMap(SceneRenderManager *manager, Light *light) {
	if (light->Type() == Light::DirectionalLight) {
		return std::shared_ptr<ShadowMapDirectional>(new ShadowMapDirectional(manager, light));
	}

	return nullptr;
}

}
