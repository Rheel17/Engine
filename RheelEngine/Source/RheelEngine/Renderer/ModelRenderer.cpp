#include "ModelRenderer.h"

#include <tuple>

#include "../Resources.h"
#include "../Engine.h"

namespace rheel {

GLShaderProgram ModelRenderer::_model_shader;
bool ModelRenderer::_is_model_shader_initialized = false;

ModelRenderer::ObjectData::ObjectData() :
		_model_matrix(glm::identity<mat4>()),
		_normal_model_matrix(glm::identity<mat4>()) {}

void ModelRenderer::ObjectData::SetTransform(vec3 position, quat rotation, vec3 scale) {
	_model_matrix = glm::translate(glm::mat4_cast(rotation) * glm::scale(glm::identity<mat4>(), scale), position);
	_normal_model_matrix = glm::transpose(glm::inverse(_model_matrix));
}

void ModelRenderer::ObjectData::SetMaterialVector(vec4 materialVector) {
	_material_vector = std::move(materialVector);
}

void ModelRenderer::ObjectData::SetMaterialColor(vec4 materialColor) {
	_material_color = std::move(materialColor);
}

ModelRenderer::_ObjectDataList::~_ObjectDataList() {
	for (ObjectData *data : _objects) {
		delete data;
	}
}

ModelRenderer::ObjectData *ModelRenderer::_ObjectDataList::Add() {
	ObjectData *data = new ObjectData;
	_objects.insert(data);
	return data;
}

void ModelRenderer::_ObjectDataList::Remove(ObjectData *data) {
	_objects.erase(data);
	delete data;
}

std::vector<ModelRenderer::ObjectData> ModelRenderer::_ObjectDataList::Data() const {
	std::vector<ObjectData> vec;
	vec.reserve(_objects.size());

	for (auto data : _objects) {
		vec.push_back(*data);
	}

	return vec;
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

ModelRenderer::ObjectData *ModelRenderer::AddObject() {
	return _objects.Add();
}

ModelRenderer::ObjectData *ModelRenderer::AddTexturedObject(const Material& material) {
	return _textured_objects[material].Add();
}

void ModelRenderer::RemoveObject(ObjectData *object) {
	_objects.Remove(object);
}

void ModelRenderer::RemoveTexturedObject(const Material& material, ObjectData *object) {
	_textured_objects[material].Remove(object);
}

GLShaderProgram& ModelRenderer::GetModelShader() {
	_InitializeShader();
	return _model_shader;
}

void ModelRenderer::RenderObjects() const {
	_vao.Bind();

	auto data = _objects.Data();
	_object_data_buffer.SetData(data, GLBuffer::STREAM_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, data.size());

	for (auto pair : _textured_objects) {
		pair.first.BindTextures();

		auto pairData = pair.second.Data();
		_object_data_buffer.SetData(pairData, GLBuffer::STREAM_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, pairData.size());
	}
}

void ModelRenderer::_InitializeShader() {
	if (_is_model_shader_initialized) {
		return;
	}

	_model_shader.AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_modelshader_vert_glsl));
	_model_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_modelshader_frag_glsl));
	_model_shader.Link();
	_model_shader["ambientTexture"] = 0;
	_model_shader["diffuseTexture"] = 1;
	_model_shader["specularTexture"] = 2;

	_is_model_shader_initialized = true;
}

}
