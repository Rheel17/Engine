/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SCENERENDERER_H
#define RHEELENGINE_SCENERENDERER_H
#include "../_common.h"

#include "SceneRenderManager.h"
#include "ShadowMap.h"
#include "OpenGL/Framebuffer.h"
#include "../Components/Camera.h"

namespace rheel {

class RE_API SceneRenderer {

public:
	virtual ~SceneRenderer() = default;

	void SetSize(unsigned width, unsigned height);

	const gl::Framebuffer& ResultBuffer() const;

	virtual void Render(float dt) = 0;

protected:
	SceneRenderer(SceneRenderManager* manager, ConstEntityId camera_entity, unsigned width, unsigned height, unsigned sample_count, bool depth_component);

	void RenderShadowMaps();

	void RenderSkybox(unsigned width, unsigned height);

	SceneRenderManager* GetManager() const;

	const Camera* GetCamera() const;

	unsigned Width() const;

	unsigned Height() const;

	const std::map<const Light*, std::unique_ptr<ShadowMap>>& ShadowMaps() const;

	virtual void Resize(unsigned width, unsigned height) = 0;

private:
	void _correct_shadow_map_list();

	SceneRenderManager* _manager;
	ConstEntityId _camera_entity;
	unsigned _width;
	unsigned _height;

	gl::Framebuffer _result_buffer;

	std::map<const Light*, std::unique_ptr<ShadowMap>> _shadow_maps;

};

}

#endif
