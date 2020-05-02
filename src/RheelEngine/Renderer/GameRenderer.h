/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GAMERENDERER_H
#define RHEELENGINE_GAMERENDERER_H
#include "../_common.h"

#include "SceneRenderManager.h"
#include "../Util/Cache.h"

namespace rheel {

class RE_API GameRenderer {

public:
	SceneRenderManager& GetSceneRenderManager(Scene* scene);

private:
	Cache<Scene*, std::unique_ptr<SceneRenderManager>> _scene_render_managers;

};

}

#endif
