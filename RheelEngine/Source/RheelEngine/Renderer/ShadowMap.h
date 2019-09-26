/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "../_common.h"

#include "SceneRenderManager.h"

namespace rheel {

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
