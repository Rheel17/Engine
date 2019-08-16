#include "ModelRenderer.h"

#include <algorithm>
#include <tuple>

#include "../Resources.h"
#include "../Engine.h"

namespace rheel {

GLShaderProgram ModelRenderer::_model_shader;
GLShaderProgram ModelRenderer::_opaque_shader;
bool ModelRenderer::_are_shaders_initialized = false;

ModelRenderer::ObjectData::ObjectData() :
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

ModelRenderer::ObjectData& ModelRenderer::ObjectData::operator=(ObjectData&& data) {
	_model_matrix = std::move(data._model_matrix);
	_normal_model_matrix = std::move(data._normal_model_matrix);
	_material_vector = std::move(data._material_vector);
	_material_color = std::move(data._material_color);
	_ptr = data._ptr;

	if (_ptr) {
		_ptr->_data = this;
	}

	return *this;
}

ModelRenderer::ObjectDataPtr::ObjectDataPtr() :
		_data(nullptr) {}

ModelRenderer::ObjectDataPtr::ObjectDataPtr(ObjectData *data) :
		_data(data) {

	_data->_ptr = this;
}

void ModelRenderer::ObjectDataPtr::SetTransform(vec3 position, quat rotation, vec3 scale) {
	_data->_model_matrix = glm::translate(glm::mat4_cast(rotation) * glm::scale(glm::identity<mat4>(), scale), position);
	_data->_normal_model_matrix = glm::transpose(glm::inverse(_data->_model_matrix));
}

void ModelRenderer::ObjectDataPtr::SetMaterialVector(vec4 materialVector) {
	_data->_material_vector = std::move(materialVector);
}

void ModelRenderer::ObjectDataPtr::SetMaterialColor(vec4 materialColor) {
	_data->_material_color = std::move(materialColor);
}

ModelRenderer::ObjectDataPtr::operator bool() const {
	return _data;
}

ModelRenderer::ObjectDataPtr& ModelRenderer::ObjectDataPtr::operator=(ObjectDataPtr&& ptr) {
	_data = std::move(ptr._data);
	_data->_ptr = this;
	return *this;
}

bool ModelRenderer::_MaterialTextureCompare::operator()(const Material& mat1, const Material& mat2) const {
	return std::tie(mat1.AmbientTexture(), mat1.DiffuseTexture(), mat1.SpecularTexture()) <
			std::tie(mat2.AmbientTexture(), mat2.DiffuseTexture(), mat2.SpecularTexture());
}

ModelRenderer::ModelRenderer(ModelPtr model) :
		_vertex_buffer_object(GL::BufferTarget::ARRAY),
		_element_array_buffer(GL::BufferTarget::ELEMENT_ARRAY),
		_object_data_buffer(GL::BufferTarget::ARRAY),
		_index_count(model->Indices().size()) {

	_vertex_buffer_object.SetData(model->Vertices());
	_element_array_buffer.SetData(model->Indices());
	_object_data_buffer.SetData(std::vector<ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(_element_array_buffer);
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ObjectData), true);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddObject() {
	return _Add(_objects);
}

ModelRenderer::ObjectDataPtr ModelRenderer::AddTexturedObject(const Material& material) {
	return _Add(_textured_objects[material]);
}

void ModelRenderer::RemoveObject(ObjectDataPtr&& object) {
	_Remove(_objects, std::move(object));
}

void ModelRenderer::RemoveTexturedObject(const Material& material, ObjectDataPtr&& object) {
	_Remove(_textured_objects[material], std::move(object));
}

GLShaderProgram& ModelRenderer::GetModelShader() {
	_InitializeShaders();
	return _model_shader;
}

GLShaderProgram& ModelRenderer::GetOpaqueShader() {
	_InitializeShaders();
	return _opaque_shader;
}

void ModelRenderer::RenderObjects() const {
	_vao.Bind();

	_object_data_buffer.SetData(_objects, GLBuffer::STREAM_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, _objects.size());

	for (auto pair : _textured_objects) {
		pair.first.BindTextures();

		_object_data_buffer.SetData(pair.second, GLBuffer::STREAM_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, pair.second.size());
	}
}

ModelRenderer::ObjectDataPtr ModelRenderer::_Add(_ObjectDataVector& objects) {
	return &objects.emplace_back();
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

	_model_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_modelshader_vert_glsl));
	_model_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_modelshader_frag_glsl));
	_model_shader.Link();
	_model_shader["ambientTexture"] = 0;
	_model_shader["diffuseTexture"] = 1;
	_model_shader["specularTexture"] = 2;

	_opaque_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_opaqueshader_vert_glsl));
	_opaque_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_opaqueshader_frag_glsl));
	_opaque_shader.Link();

	_are_shaders_initialized = true;
}

}
