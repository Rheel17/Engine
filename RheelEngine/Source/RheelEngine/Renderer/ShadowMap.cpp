#include "ShadowMap.h"

#include "SceneRenderManager.h"

namespace rheel {

ShadowMap::ShadowMap(SceneRenderManager *manager, const std::string& light) :
		_manager(manager) {

	std::cout << "ShadowMap()" << std::endl;
}

void ShadowMap::Update() {

}

}
