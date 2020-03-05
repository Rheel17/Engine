/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SKYBOXRENDERER_H
#define RHEELENGINE_SKYBOXRENDERER_H
#include "../_common.h"

#include "OpenGL/GLShaderProgram.h"
#include "../Assets/Image.h"
#include "../Components/Camera.h"
#include "OpenGL/GLVertexArray.h"

namespace rheel {

class SceneRenderManager;

class RE_API SkyboxRenderer {

private:
	struct Vertex {
		vec3 position;
		vec3 texture;
	};

public:
	explicit SkyboxRenderer(SceneRenderManager *manager);

	void Render(Camera *camera, unsigned width, unsigned height) const;

private:
	void _LoadSkybox() const;
	void _LoadSkybox(const Image& skybox, int part) const;

	SceneRenderManager *_manager;

	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;

	GLShaderProgram _shader;

	mutable std::uintptr_t _current_image{};
	mutable std::array<Image, 6> _texture_parts;

private:
	static std::vector<Vertex> _CreateSkyboxVertices();
	static std::vector<unsigned> _CreateSkyboxIndices();

};

}

#endif
