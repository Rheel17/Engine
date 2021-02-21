/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "SkyboxRenderer.h"

#include "SceneRenderManager.h"
#include "ImageTexture.h"
#include "OpenGL/Context.h"
#include "../Components/Camera.h"
#include "../Components/Skybox.h"
#include "../EngineResources.h"

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

	gl::ContextScope cs;

	// initialize the shader
	_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_skybox_vert_glsl"));
	_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_skybox_frag_glsl"));
	_shader.Link();
	_shader["textures"] = 0;

	// initialize the skybox model
	_vertex_buffer_object.SetData(_create_skybox_vertices());
	_vao.SetVertexAttributes<vec3, vec3>(_vertex_buffer_object);
	_vao.SetVertexIndices(_create_skybox_indices());

	// initialize the texture
	_textures = _create_texture(1, 1);
}

void SkyboxRenderer::Render(const Camera* camera, unsigned width, unsigned height) const {
	// auto skybox = _manager->GetScene()->GetSkybox();
	// if (skybox == nullptr) {
		return;
	// }
	//
	// // render the skybox
	// _vao.Bind();
	//
	// _shader["projectionMatrix"] = camera->GetProjectionMatrix(width, height);
	// _shader["rotationMatrix"] = camera->GetRotationMatrix();
	// _shader["renderDistance"] = camera->GetSkyboxDistance();
	// _shader["scale"] = skybox->GetScale();
	//
	// const auto& images = skybox->GetImages();
	// bool changed = false;
	//
	// for (int i = 0; i < 6; i++) {
	// 	if (_current_images[i].GetAddress() != images[i].GetAddress()) {
	// 		// changed skybox
	// 		LoadImage_(images[i], i);
	// 		_current_images[i] = images[i];
	// 		changed = true;
	// 	}
	// }
	//
	// if (changed) {
	// 	_textures.GenerateMipmap();
	// }
	//
	// _textures.Bind(0);
	// _vao.DrawElements(gl::VertexArray::Mode::TRIANGLES, 36);
}

void SkyboxRenderer::_load_image(Image image, unsigned layer) const {
	unsigned image_width = std::max(image.GetWidth(), 1u);
	unsigned image_height = std::max(image.GetHeight(), 1u);

	if (image_width != _current_width || image_height != _current_height) {
		_current_width = image_width;
		_current_height = image_height;
		_textures = _create_texture(_current_width, _current_height);
	}

	_textures.SetLayerData(image.GetWidth(), image.GetHeight(), layer, gl::Format::RGBA, image.GetRawColorData());
}

gl::Texture2DArray SkyboxRenderer::_create_texture(unsigned width, unsigned height) {
	gl::Texture2DArray texture;

	if (DisplayConfiguration::Get().enable_mipmaps) {
		texture.SetMinifyingFilter(gl::Texture::FilterFunction::LINEAR_MIPMAP_LINEAR);
	} else {
		texture.SetMinifyingFilter(gl::Texture::FilterFunction::LINEAR);
	}

	texture.SetMagnificationFilter(gl::Texture::FilterFunction::LINEAR);
	texture.SetWrapParameterS(gl::Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetWrapParameterT(gl::Texture::WrapParameter::CLAMP_TO_EDGE);
	texture.SetAnisotropyParameter(DisplayConfiguration::Get().anisotropic_level);
	texture.Initialize(gl::InternalFormat::RGBA8, width, height, 6);

	return texture;
}

std::vector<SkyboxRenderer::vertex> SkyboxRenderer::_create_skybox_vertices() {
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

std::vector<unsigned> SkyboxRenderer::_create_skybox_indices() {
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
