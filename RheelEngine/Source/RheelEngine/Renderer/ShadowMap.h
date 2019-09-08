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

	virtual void Update(const Camera& camera, unsigned width, unsigned height) = 0;

protected:
	ShadowMap(SceneRenderManager *manager, Light& light);

	SceneRenderManager *Manager() const;

	template<typename T>
	T& GetLight() const {
		return dynamic_cast<T&>(_light);
	}

	Light& GetLight() const {
		return _light;
	}

private:
	SceneRenderManager *_manager;
	Light& _light;

};

}

#endif
