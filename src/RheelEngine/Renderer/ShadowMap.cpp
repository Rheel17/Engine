/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ShadowMap.h"

namespace rheel {

ShadowMap::ShadowMap(SceneRenderManager* manager, const Light& light) :
		_manager(manager),
		_light(light) {}

SceneRenderManager* ShadowMap::GetManager() const {
	return _manager;
}

const Light& ShadowMap::GetLight() const {
	return _light;
}

}
