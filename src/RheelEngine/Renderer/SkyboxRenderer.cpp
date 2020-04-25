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
		_vertex_buffer_object(gl::Buffer::Target::ARRAY) {

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

	// render the skybox
	_vao.Bind();

	_shader["projectionMatrix"] = camera->GetProjectionMatrix(width, height);
	_shader["rotationMatrix"] = camera->GetRotationMatrix();
	_shader["renderDistance"] = camera->GetSkyboxDistance();
	_shader["scale"] = skybox->GetScale();

	const auto& images = skybox->GetImages();

	for (int i = 0; i < 6; i++) {
		if (images[i].IsNull()) {
			gl::State::ClearTexture(i, gl::Texture::Target::TEXTURE_2D);
		} else {
			ImageTexture::Get(images[i]).Bind(i);
		}
	}

	_vao.DrawElements(gl::VertexArray::Mode::TRIANGLES, 36);
}

std::vector<SkyboxRenderer::vertex> SkyboxRenderer::CreateSkyboxVertices_() {
	vec3 position0 = { -1.0f, -1.0f, -1.0f };
	vec3 position1 = { -1.0f, -1.0f,  1.0f };
	vec3 position2 = { -1.0f,  1.0f, -1.0f };
	vec3 position3 = { -1.0f,  1.0f,  1.0f };
	vec3 position4 = {  1.0f, -1.0f, -1.0f };
	vec3 position5 = {  1.0f, -1.0f,  1.0f };
	vec3 position6 = {  1.0f,  1.0f, -1.0f };
	vec3 position7 = {  1.0f,  1.0f,  1.0f };

	return {
			{ position0, { 1.0f, 1.0f, PART_WEST } },
			{ position1, { 0.0f, 1.0f, PART_WEST } },
			{ position2, { 1.0f, 0.0f, PART_WEST } },
			{ position3, { 0.0f, 0.0f, PART_WEST } },

			{ position4, { 0.0f, 1.0f, PART_EAST } },
			{ position5, { 1.0f, 1.0f, PART_EAST } },
			{ position6, { 0.0f, 0.0f, PART_EAST } },
			{ position7, { 1.0f, 0.0f, PART_EAST } },

			{ position0, { 0.0f, 0.0f, PART_DOWN } },
			{ position4, { 1.0f, 0.0f, PART_DOWN } },
			{ position5, { 1.0f, 1.0f, PART_DOWN } },
			{ position1, { 0.0f, 1.0f, PART_DOWN } },

			{ position2, { 0.0f, 1.0f, PART_UP } },
			{ position3, { 0.0f, 0.0f, PART_UP } },
			{ position6, { 1.0f, 1.0f, PART_UP } },
			{ position7, { 1.0f, 0.0f, PART_UP } },

			{ position0, { 0.0f, 1.0f, PART_NORTH } },
			{ position2, { 0.0f, 0.0f, PART_NORTH } },
			{ position6, { 1.0f, 0.0f, PART_NORTH } },
			{ position4, { 1.0f, 1.0f, PART_NORTH } },

			{ position1, { 1.0f, 1.0f, PART_SOUTH } },
			{ position5, { 0.0f, 1.0f, PART_SOUTH } },
			{ position3, { 1.0f, 0.0f, PART_SOUTH } },
			{ position7, { 0.0f, 0.0f, PART_SOUTH } }
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
