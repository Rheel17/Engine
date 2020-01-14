/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "../_common.h"

#include "SceneRenderManager.h"
#include "../Components/Camera.h"
#include "../Components/Light.h"

namespace rheel {

class RE_API ShadowMap {
	friend class SceneRenderManager;

public:
	virtual ~ShadowMap() = default;

	virtual void Update(Camera *camera, unsigned width, unsigned height) = 0;

protected:
	ShadowMap(SceneRenderManager *manager, Light *light);

	SceneRenderManager *GetManager() const;

	template<typename T>
	T *GetLight() const {
		return dynamic_cast<T *>(_light);
	}

	Light *GetLight() const;

private:
	SceneRenderManager *_manager;
	Light *_light;

};

}

#endif
