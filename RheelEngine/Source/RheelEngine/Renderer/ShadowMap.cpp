#include "ShadowMap.h"

namespace rheel {

ShadowMap::ShadowMap(SceneRenderManager *manager, LightPtr light) :
		_manager(manager), _light(light) {}

SceneRenderManager *ShadowMap::Manager() const {
	return _manager;
}

}
