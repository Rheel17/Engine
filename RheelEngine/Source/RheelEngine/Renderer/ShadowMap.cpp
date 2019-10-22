/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ShadowMap.h"

namespace rheel {

ShadowMap::ShadowMap(SceneRenderManager *manager, Light *light) :
		_manager(manager), _light(light) {}

SceneRenderManager *ShadowMap::GetManager() const {
	return _manager;
}

Light *ShadowMap::GetLight() const {
	return _light;
}

}
