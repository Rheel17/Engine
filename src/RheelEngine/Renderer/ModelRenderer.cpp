/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelRenderer.h"

#include "OpenGL/Context.h"

#include <array>

namespace rheel {

ModelRenderer::ObjectData::ObjectData() :
		_model_matrix(glm::identity<mat4>()),
		_normal_model_matrix(glm::identity<mat4>()),
		_material_vector(0, 0, 0, 0),
		_material_color(0, 0, 0, 0) {}

ModelRenderer::ObjectData::ObjectData(ObjectData&& data) noexcept :
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
	if (this == &data) {
		return *this;
	}

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

ModelRenderer::ObjectDataPtr::ObjectDataPtr(ObjectData* data) :
		_data(data) {

	_data->_ptr = this;
}

void ModelRenderer::ObjectDataPtr::SetMatrix(mat4 matrix) {
	_data->_model_matrix = matrix;
	_data->_normal_model_matrix = glm::transpose(glm::inverse(mat3(_data->_model_matrix)));
}

void ModelRenderer::ObjectDataPtr::SetMaterialVector(vec4 material_vector) {
	_data->_material_vector = material_vector;
}

void ModelRenderer::ObjectDataPtr::SetMaterialColor(vec4 material_color) {
	_data->_material_color = material_color;
}

ModelRenderer::ObjectDataPtr::operator bool() const {
	return _data != nullptr;
}

ModelRenderer::ObjectDataPtr::ObjectDataPtr(ModelRenderer::ObjectDataPtr&& ptr) noexcept :
		_data(ptr._data) {

	if (_data) {
		_data->_ptr = this;
	}
}

ModelRenderer::ObjectDataPtr& ModelRenderer::ObjectDataPtr::operator=(ObjectDataPtr&& ptr) noexcept {
	_data = ptr._data;

	if (_data) {
		_data->_ptr = this;
	}

	return *this;
}

bool ModelRenderer::material_texture_compare::operator()(const Material& mat_1, const Material& mat_2) const {
	std::array<std::uintptr_t, 3> mat_1_addresses = {
			{
					mat_1.GetAmbientTexture().GetAddress(),
					mat_1.GetDiffuseTexture().GetAddress(),
					mat_1.GetSpecularTexture().GetAddress()
			}
	};
	std::array<std::uintptr_t, 3> mat_2_addresses = {
			{
					mat_2.GetAmbientTexture().GetAddress(),
					mat_2.GetDiffuseTexture().GetAddress(),
					mat_2.GetSpecularTexture().GetAddress()
			}
	};
	return std::tie(mat_1_addresses[0], mat_1_addresses[1], mat_1_addresses[2]) < std::tie(mat_2_addresses[0], mat_2_addresses[1], mat_2_addresses[2]);
}

ModelRenderer::ModelRenderer(const Model& model) :
		_mode(_get_mode(model.GetRenderType())),
		_vertex_buffer_object(gl::Buffer::Target::ARRAY),
		_object_data_buffer(gl::Buffer::Target::ARRAY) {

	gl::ContextScope cs;

	_vertex_buffer_object.SetData(model.GetVertices());
	_object_data_buffer.SetData(std::vector<ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(model.GetIndices());
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ObjectData), 1);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddObject() {
	return _add(_objects);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddTexturedObject(const Material& material) {
	return _add(_textured_objects[material]);
}

void ModelRenderer::RemoveObject(ObjectDataPtr&& object) {
	_remove(_objects, std::forward<ObjectDataPtr>(object));
}

void ModelRenderer::RemoveTexturedObject(const Material& material, ObjectDataPtr&& object) {
	_remove(_textured_objects[material], std::forward<ObjectDataPtr>(object));
}

void ModelRenderer::RenderObjects() const {
	gl::Context::Current().ClearTexture(0, gl::Texture::Target::TEXTURE_2D);
	gl::Context::Current().ClearTexture(1, gl::Texture::Target::TEXTURE_2D);
	gl::Context::Current().ClearTexture(2, gl::Texture::Target::TEXTURE_2D);

	_object_data_buffer.SetData(_objects, gl::Buffer::Usage::STREAM_DRAW);
	_vao.DrawElements(_mode, _objects.size());

	for (const auto&[material, objects] : _textured_objects) {
		material.BindTextures();

		_object_data_buffer.SetData(objects, gl::Buffer::Usage::STREAM_DRAW);
		_vao.DrawElements(_mode, _objects.size());
	}
}

gl::VertexArray::Mode ModelRenderer::_get_mode(RenderType type) {
	switch (type) {
		case RenderType::TRIANGLES:
			return gl::VertexArray::Mode::TRIANGLES;
		case RenderType::LINES:
			return gl::VertexArray::Mode::LINES;
	}

	return gl::VertexArray::Mode::TRIANGLES;
}

ModelRenderer::ObjectDataPtr ModelRenderer::_add(ObjectDataVector& objects) {
	return ObjectDataPtr(&objects.emplace_back());
}

void ModelRenderer::_remove(ObjectDataVector& objects, ObjectDataPtr&& data) {
	size_t index = (((size_t) data._data) - ((size_t) &objects.front())) / sizeof(ObjectData);

	if (index >= objects.size()) {
		throw std::invalid_argument("index of ObjectData out of range");
	}

	objects.erase(objects.begin() + index);
}

}
