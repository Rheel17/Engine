/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "SkyboxRenderer.h"

#include "SceneRenderManager.h"
#include "ImageTexture.h"
#include "../EngineResources.h"
#include "../Components/Skybox.h"
#include "OpenGL/State.h"

#define PART_NORTH   0
#define PART_EAST    1
#define PART_SOUTH   2
#define PART_WEST    3
#define PART_UP      4
#define PART_DOWN    5

namespace rheel {

SkyboxRenderer::SkyboxRenderer(SceneRenderManager* manager) :
		_manager(manager),
		_vertex_buffer_object(gl::Buffer::Target::ARRAY),
		_texture_parts({ { Image::Null(), Image::Null(), Image::Null(), Image::Null(), Image::Null(), Image::Null() } }) {

	// initialize the shader
	_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_skybox_vert_glsl"));
	_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_skybox_frag_glsl"));
	_shader.Link();
	_shader["textures"] = std::vector<GLint>{ 0, 1, 2, 3, 4, 5 };

	// initialize the skybox model
	_vertex_buffer_object.SetData(CreateSkyboxVertices_());
	_vao.SetVertexAttributes<vec3, vec3>(_vertex_buffer_object);
	_vao.SetVertexIndices(CreateSkyboxIndices_());
}

void SkyboxRenderer::Render(Camera* camera, unsigned width, unsigned height) const {
	auto skybox = _manager->GetScene()->GetSkybox();
	if (skybox == nullptr) {
		return;
	}

	// check if there is a new skybox image
	auto skyboxAddress = skybox->GetImage().GetAddress();

	if (skyboxAddress != _current_image) {
		if (skyboxAddress != 0) {
			LoadSkybox_();
		}

		_current_image = skyboxAddress;
	}

	if (skyboxAddress != 0) {
		// render the skybox
		_vao.Bind();

		_shader["projectionMatrix"] = camera->GetProjectionMatrix(width, height);
		_shader["rotationMatrix"] = camera->GetRotationMatrix();
		_shader["renderDistance"] = camera->GetSkyboxDistance();
		_shader["scale"] = skybox->GetScale();

		for (int i = 0; i < 6; i++) {
			ImageTexture::Get(_texture_parts[i]).Bind(i);
		}

		_vao.DrawElements(gl::VertexArray::Mode::TRIANGLES, 36);

		for (int i = 0; i < 6; i++) {
			gl::State::ClearTexture(i, gl::Texture::Target::TEXTURE_2D);
		}
	}
}

void SkyboxRenderer::LoadSkybox_() const {
	Image skybox = _manager->GetScene()->GetSkybox()->GetImage();

	for (int i = 0; i < 6; i++) {
		LoadSkybox_(skybox, i);
	}
}

void SkyboxRenderer::LoadSkybox_(const Image& skybox, int part) const {
	// get the coordinates of the texture in the skybox
	unsigned size = skybox.GetWidth() / 4;
	unsigned x = 0;
	unsigned y = 0;

	switch (part) {
		case PART_NORTH:
			x = size;
			y = size;
			break;
		case PART_EAST:
			x = 2 * size;
			y = size;
			break;
		case PART_SOUTH:
			x = 3 * size;
			y = size;
			break;
		case PART_WEST:
			x = 0;
			y = size;
			break;
		case PART_UP:
			x = size;
			y = 0;
			break;
		case PART_DOWN:
			x = size;
			y = 2 * size;
			break;
		default:
			abort();
	}

	// extract the texture
	_texture_parts[part] = skybox.SubImage(x, y, size, size);
}

std::vector<SkyboxRenderer::vertex> SkyboxRenderer::CreateSkyboxVertices_() {
	vec3 position_0 = { -1.0f, -1.0f, -1.0f };
	vec3 position_1 = { -1.0f, -1.0f,  1.0f };
	vec3 position_2 = { -1.0f,  1.0f, -1.0f };
	vec3 position_3 = { -1.0f,  1.0f,  1.0f };
	vec3 position_4 = {  1.0f, -1.0f, -1.0f };
	vec3 position_5 = {  1.0f, -1.0f,  1.0f };
	vec3 position_6 = {  1.0f,  1.0f, -1.0f };
	vec3 position_7 = {  1.0f,  1.0f,  1.0f };

	return {
			{ position_0, { 1.0f, 1.0f, PART_WEST } },
			{ position_1, { 0.0f, 1.0f, PART_WEST } },
			{ position_2, { 1.0f, 0.0f, PART_WEST } },
			{ position_3, { 0.0f, 0.0f, PART_WEST } },

			{ position_4, { 0.0f, 1.0f, PART_EAST } },
			{ position_5, { 1.0f, 1.0f, PART_EAST } },
			{ position_6, { 0.0f, 0.0f, PART_EAST } },
			{ position_7, { 1.0f, 0.0f, PART_EAST } },

			{ position_0, { 0.0f, 0.0f, PART_DOWN } },
			{ position_4, { 1.0f, 0.0f, PART_DOWN } },
			{ position_5, { 1.0f, 1.0f, PART_DOWN } },
			{ position_1, { 0.0f, 1.0f, PART_DOWN } },

			{ position_2, { 0.0f, 1.0f, PART_UP } },
			{ position_3, { 0.0f, 0.0f, PART_UP } },
			{ position_6, { 1.0f, 1.0f, PART_UP } },
			{ position_7, { 1.0f, 0.0f, PART_UP } },

			{ position_0, { 0.0f, 1.0f, PART_NORTH } },
			{ position_2, { 0.0f, 0.0f, PART_NORTH } },
			{ position_6, { 1.0f, 0.0f, PART_NORTH } },
			{ position_4, { 1.0f, 1.0f, PART_NORTH } },

			{ position_1, { 1.0f, 1.0f, PART_SOUTH } },
			{ position_5, { 0.0f, 1.0f, PART_SOUTH } },
			{ position_3, { 1.0f, 0.0f, PART_SOUTH } },
			{ position_7, { 0.0f, 0.0f, PART_SOUTH } }
	};
}

std::vector<unsigned> SkyboxRenderer::CreateSkyboxIndices_() {
	return std::vector<GLuint>{
			 2,  3,  1,  2,  1,  0,
			 7,  6,  4,  5,  7,  4,
			11, 10,  9, 11,  9,  8,
			12, 14, 15, 15, 13, 12,
			18, 17, 19, 19, 17, 16,
			23, 21, 20, 22, 23, 20
	};
}

}
