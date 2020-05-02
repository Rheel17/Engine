/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "GameRenderer.h"

namespace rheel {

SceneRenderManager& GameRenderer::GetSceneRenderManager(Scene* scene) {
	return *_scene_render_managers.Get(scene, [](Scene* s) { return std::make_unique<SceneRenderManager>(s); });
}

}