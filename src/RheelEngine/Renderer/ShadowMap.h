/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SHADOWMAP_H
#define RHEELENGINE_SHADOWMAP_H
#include "../_common.h"

#include "SceneRenderManager.h"
#include "../Components/Camera.h"
#include "../Components/Light.h"

namespace rheel {

class RE_API ShadowMap {
	friend class SceneRenderManager;

public:
	virtual ~ShadowMap() = default;

	virtual void Update(const Camera* camera, unsigned width, unsigned height) = 0;

protected:
	ShadowMap(SceneRenderManager* manager, const Light& light);

	SceneRenderManager* GetManager() const;

	template<typename T>
	const T& GetLight() const {
		return dynamic_cast<const T&>(_light);
	}

	const Light& GetLight() const;

private:
	SceneRenderManager* _manager;
	const Light& _light;

};

}

#endif
