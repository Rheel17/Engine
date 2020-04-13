/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderer.h"

#include "OpenGL/State.h"
#include "../EngineResources.h"

#include <array>

namespace rheel {

GL::Program ModelRenderer::_forward_model_shader;
GL::Program ModelRenderer::_opaque_shader;
bool ModelRenderer::_are_shaders_initialized = false;

ModelRenderer::ObjectData::ObjectData() :
		_model_matrix(glm::identity<mat4>()),
		_normal_model_matrix(glm::identity<mat4>()),
		_material_vector(0, 0, 0, 0),
		_material_color(0, 0, 0, 0),
		_ptr(nullptr) {}

ModelRenderer::ObjectData::ObjectData(const ObjectData& data) :
		_model_matrix(data._model_matrix),
		_normal_model_matrix(data._normal_model_matrix),
		_material_vector(data._material_vector),
		_material_color(data._material_color),
		_ptr(data._ptr) {

	if (_ptr) {
		_ptr->_data = this;
	}
}

ModelRenderer::ObjectData& ModelRenderer::ObjectData::operator=(ObjectData&& data) noexcept {
	_model_matrix = data._model_matrix;
	_normal_model_matrix = data._normal_model_matrix;
	_material_vector = data._material_vector;
	_material_color = data._material_color;
	_ptr = data._ptr;

	if (_ptr) {
		_ptr->_data = this;
	}

	return *this;
}

ModelRenderer::ObjectDataPtr::ObjectDataPtr(ObjectData *data) :
		_data(data) {

	_data->_ptr = this;
}

void ModelRenderer::ObjectDataPtr::SetMatrix(mat4 matrix) {
	_data->_model_matrix = matrix;
	_data->_normal_model_matrix = glm::transpose(glm::inverse(mat3(_data->_model_matrix)));
}

void ModelRenderer::ObjectDataPtr::SetMaterialVector(vec4 materialVector) {
	_data->_material_vector = materialVector;
}

void ModelRenderer::ObjectDataPtr::SetMaterialColor(vec4 materialColor) {
	_data->_material_color = materialColor;
}

ModelRenderer::ObjectDataPtr::operator bool() const {
	return _data != nullptr;
}

ModelRenderer::ObjectDataPtr& ModelRenderer::ObjectDataPtr::operator=(ObjectDataPtr&& ptr) noexcept {
	_data = ptr._data;
	_data->_ptr = this;
	return *this;
}

bool ModelRenderer::_MaterialTextureCompare::operator()(const Material& mat1, const Material& mat2) const {
	std::array<std::uintptr_t, 3> mat1Addresses = {{
			mat1.GetAmbientTexture().GetAddress(),
			mat1.GetDiffuseTexture().GetAddress(),
			mat1.GetSpecularTexture().GetAddress() }};
	std::array<std::uintptr_t, 3> mat2Addresses = {{
			mat2.GetAmbientTexture().GetAddress(),
			mat2.GetDiffuseTexture().GetAddress(),
			mat2.GetSpecularTexture().GetAddress() }};

	return std::tie(mat1Addresses[0], mat1Addresses[1], mat1Addresses[2]) <
			std::tie(mat2Addresses[0], mat2Addresses[1], mat2Addresses[2]);
}

bool ModelRenderer::_MaterialShaderCompare::operator()(const Material& mat1, const Material& mat2) const {
	return mat1.GetCustomShader().GetAddress() < mat2.GetCustomShader().GetAddress();
}

ModelRenderer::ModelRenderer(const Model& model) :
		_vertex_buffer_object(GL::Buffer::Target::ARRAY),
		_object_data_buffer(GL::Buffer::Target::ARRAY) {

	_vertex_buffer_object.SetData(model.GetVertices());
	_object_data_buffer.SetData(std::vector<ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(model.GetIndices());
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ObjectData), true);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddObject() {
	return _Add(_objects);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddTexturedObject(const Material& material) {
	return _Add(_textured_objects[material]);
}

void ModelRenderer::RemoveObject(ObjectDataPtr&& object) {
	_Remove(_objects, std::forward<ObjectDataPtr>(object));
}

void ModelRenderer::RemoveTexturedObject(const Material& material, ObjectDataPtr&& object) {
	_Remove(_textured_objects[material], std::forward<ObjectDataPtr>(object));
}

GL::Program& ModelRenderer::GetForwardModelShader() {
	_InitializeShaders();
	return _forward_model_shader;
}

GL::Program& ModelRenderer::GetOpaqueShader() {
	_InitializeShaders();
	return _opaque_shader;
}

void ModelRenderer::RenderObjects() const {
	GL::State::ClearTexture(0, GL::Texture::Target::TEXTURE_2D);
	GL::State::ClearTexture(1, GL::Texture::Target::TEXTURE_2D);
	GL::State::ClearTexture(2, GL::Texture::Target::TEXTURE_2D);

	_object_data_buffer.SetData(_objects, GL::Buffer::Usage::STREAM_DRAW);
	_vao.DrawElements(GL::VertexArray::Mode::TRIANGLES, _objects.size());

	for (const auto& [material, objects] : _textured_objects) {
		material.BindTextures();

		_object_data_buffer.SetData(objects, GL::Buffer::Usage::STREAM_DRAW);
		_vao.DrawElements(GL::VertexArray::Mode::TRIANGLES, _objects.size());
	}
}

ModelRenderer::ObjectDataPtr ModelRenderer::_Add(_ObjectDataVector& objects) {
	return ObjectDataPtr(&objects.emplace_back());
}

void ModelRenderer::_Remove(_ObjectDataVector& objects, ObjectDataPtr&& data) {
	size_t index = (((size_t) data._data) - ((size_t) &objects.front())) / sizeof(ObjectData);

	if (index >= objects.size()) {
		throw std::invalid_argument("index of ObjectData out of range");
	}

	objects.erase(objects.begin() + index);
}

void ModelRenderer::_InitializeShaders() {
	if (_are_shaders_initialized) {
		return;
	}

	_forward_model_shader.AttachShader(GL::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_modelshader_vert_glsl"));
	_forward_model_shader.AttachShader(GL::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_modelshader_frag_glsl"));
	_forward_model_shader.Link();
	_forward_model_shader["ambientTexture"] = 0;
	_forward_model_shader["diffuseTexture"] = 1;
	_forward_model_shader["specularTexture"] = 2;
	_forward_model_shader["_shadowMap0"] = 3;
	_forward_model_shader["_shadowMap1"] = 4;
	_forward_model_shader["_shadowMap2"] = 5;
	_forward_model_shader["_shadowMap3"] = 6;

	_opaque_shader.AttachShader(GL::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_opaqueshader_vert_glsl"));
	_opaque_shader.AttachShader(GL::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_opaqueshader_frag_glsl"));
	_opaque_shader.Link();

	_are_shaders_initialized = true;
}

}
