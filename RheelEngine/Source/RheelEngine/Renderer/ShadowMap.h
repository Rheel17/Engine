#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "../_common.h"

#include <vector>

#include "OpenGL/GLFramebuffer.h"
#include "../Light.h"
#include "../Camera.h"

namespace rheel {

class SceneRenderManager;

class RE_API ShadowMap {
	friend class SceneRenderManager;

public:
	virtual ~ShadowMap() = default;

	virtual void Update(const mat4& cameraMatrix, const mat4& cameraMatrixInv) = 0;

protected:
	ShadowMap(SceneRenderManager *manager, Light *light);

	Light *GetLight() const;

private:
	SceneRenderManager *_manager;
	Light *_light;

public:
	static std::shared_ptr<ShadowMap> CreateShadowMap(SceneRenderManager *manager, Light *lightName);

};

}

#endif
