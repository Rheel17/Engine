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

	struct RE_API empty_shadow_map {
		empty_shadow_map();
		gl::Texture2D texture;
	};
	pseudo_static_pointer<empty_shadow_map> _empty_shadow_map;

public:
	void Render(float dt) override;

protected:
	void Resize(unsigned width, unsigned height) override;

private:
	ForwardSceneRenderer(SceneRenderManager* manager, ConstEntityId camera_entity, unsigned width, unsigned height, unsigned sample_count);

};

}

#endif
