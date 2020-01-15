/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SCENERENDERER_H_
#define SCENERENDERER_H_
#include "../_common.h"

#include "SceneRenderManager.h"
#include "ShadowMap.h"
#include "OpenGL/GLFramebuffer.h"
#include "../Components/Camera.h"

namespace rheel {

class RE_API SceneRenderer {

public:
	virtual ~SceneRenderer();

	void SetSize(unsigned width, unsigned height);

	const GLFramebuffer& ResultBuffer() const;

	virtual void Render(float dt) = 0;

protected:
	SceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height, unsigned samples, bool depthComponent);

	void _RenderShadowMaps();

	SceneRenderManager *GetManager() const;

	Camera *GetCamera() const;

	unsigned Width() const;

	unsigned Height() const;

	const std::unordered_map<Light *, std::unique_ptr<ShadowMap>>& ShadowMaps() const;

	virtual void Resize(unsigned width, unsigned height) = 0;

private:
	void _CorrectShadowMapList();

	SceneRenderManager *_manager;
	std::string _camera_name;
	unsigned _width;
	unsigned _height;

	GLFramebuffer _result_buffer;

	std::unordered_map<Light *, std::unique_ptr<ShadowMap>> _shadow_maps;

};

}

#endif