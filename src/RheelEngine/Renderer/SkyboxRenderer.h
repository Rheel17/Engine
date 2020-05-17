/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SKYBOXRENDERER_H
#define RHEELENGINE_SKYBOXRENDERER_H
#include "../_common.h"

#include "OpenGL/Program.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Texture2DArray.h"
#include "../Assets/Image.h"

namespace rheel {

class Camera;
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
	void LoadImage_(Image image, unsigned layer) const;

	SceneRenderManager* _manager;

	gl::VertexArray _vao;
	gl::Buffer _vertex_buffer_object;
	gl::Program _shader;
	mutable gl::Texture2DArray _textures;

	mutable std::array<Image, 6> _current_images{
		Image::Null(), Image::Null(), Image::Null(),
		Image::Null(), Image::Null(), Image::Null()
	};

	mutable unsigned _current_width = 1;
	mutable unsigned _current_height = 1;

private:
	static gl::Texture2DArray CreateTexture_(unsigned width, unsigned height);
	static std::vector<vertex> CreateSkyboxVertices_();
	static std::vector<unsigned> CreateSkyboxIndices_();

};

}

#endif
