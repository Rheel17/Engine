#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "../_common.h"

#include <vector>

#include "OpenGL/GLTexture2D.h"

namespace rheel {

class SceneRenderManager;

class RE_API ShadowMap {

public:
	ShadowMap(SceneRenderManager *manager);

	void Update();

private:
	SceneRenderManager *_manager;
	std::vector<GLTexture2D> _shadow_textures;
};

}

#endif
