/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef FORWARDSCENERENDERER_H_
#define FORWARDSCENERENDERER_H_
#include "../_common.h"

#include "SceneRenderer.h"

namespace rheel {

class RE_API ForwardSceneRenderer : public SceneRenderer {
	friend class SceneRenderManager;

public:
	void Render(float dt) override;

protected:
	GLFramebuffer CreateResultBuffer(unsigned width, unsigned height) const;

	void Resize(unsigned width, unsigned height) override;

private:
	ForwardSceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height);

};

}

#endif
