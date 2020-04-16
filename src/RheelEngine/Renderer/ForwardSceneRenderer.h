/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_FORWARDSCENERENDERER_H
#define RHEELENGINE_FORWARDSCENERENDERER_H
#include "../_common.h"

#include "SceneRenderer.h"

namespace rheel {

class RE_API ForwardSceneRenderer : public SceneRenderer {
	friend class SceneRenderManager;

public:
	void Render(float dt) override;

protected:
	void Resize(unsigned width, unsigned height) override;

private:
	ForwardSceneRenderer(SceneRenderManager* manager, std::string cameraName, unsigned width, unsigned height);

};

}

#endif
