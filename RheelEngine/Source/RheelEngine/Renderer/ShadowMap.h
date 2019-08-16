#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "../_common.h"

#include <vector>

#include "OpenGL/GLFramebuffer.h"

namespace rheel {

class SceneRenderManager;

class RE_API ShadowMap {
	friend class SceneRenderManager;

public:
	ShadowMap(SceneRenderManager *manager, const std::string& light);

	void Update();

private:
	SceneRenderManager *_manager;
	std::shared_ptr<GLFramebuffer> _shadow_buffer;

};

}

#endif
