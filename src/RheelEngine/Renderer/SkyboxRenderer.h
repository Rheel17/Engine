/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SKYBOXRENDERER_H
#define RHEELENGINE_SKYBOXRENDERER_H
#include "../_common.h"

#include "OpenGL/Program.h"
#include "OpenGL/VertexArray.h"
#include "../Assets/Image.h"
#include "../Components/Camera.h"

namespace rheel {

class SceneRenderManager;

class RE_API SkyboxRenderer {

private:
	struct vertex {
		vec3 position;
		vec3 texture;
	};

public:
	explicit SkyboxRenderer(SceneRenderManager* manager);

	void Render(Camera* camera, unsigned width, unsigned height) const;

private:
	SceneRenderManager* _manager;

	gl::VertexArray _vao;
	gl::Buffer _vertex_buffer_object;

	gl::Program _shader;

private:
	static std::vector<vertex> CreateSkyboxVertices_();
	static std::vector<unsigned> CreateSkyboxIndices_();

};

}

#endif
