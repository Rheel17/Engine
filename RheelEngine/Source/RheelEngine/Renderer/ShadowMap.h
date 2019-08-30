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

	virtual void Update(CameraPtr camera, unsigned width, unsigned height) = 0;

protected:
	ShadowMap(SceneRenderManager *manager, LightPtr light);

	SceneRenderManager *Manager() const;

	template<typename T>
	std::shared_ptr<T> GetLight() const {
		return std::dynamic_pointer_cast<T>(_light);
	}

	std::shared_ptr<Light> GetLight() const {
		return _light;
	}

private:
	SceneRenderManager *_manager;
	LightPtr _light;

};

}

#endif
