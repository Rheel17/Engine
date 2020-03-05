/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "CustomShaderModelRenderer.h"

#include "../EngineResources.h"

namespace rheel {

std::unordered_map<std::uintptr_t, GLShaderProgram> CustomShaderModelRenderer::_shader_cache;

CustomShaderModelRenderer::CustomShaderModelRenderer(const Model& model, const Shader& shader) :
		_vertex_buffer_object(GL::BufferTarget::ARRAY),
		_element_array_buffer(GL::BufferTarget::ELEMENT_ARRAY),
		_object_data_buffer(GL::BufferTarget::ARRAY),
		_index_count(model.GetIndices().size()),
		_shader(_GetCompiledShader(shader)) {

	_vertex_buffer_object.SetData(model.GetVertices());
	_element_array_buffer.SetData(model.GetIndices());
	_object_data_buffer.SetData(std::vector<ModelRenderer::ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(_element_array_buffer);
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ModelRenderer::ObjectData), true);

	GL::ClearVertexArrayBinding();
}

ModelRenderer::ObjectDataPtr CustomShaderModelRenderer::AddObject() {
	return ModelRenderer::ObjectDataPtr(&_objects.emplace_back());
}

void CustomShaderModelRenderer::RemoveObject(ModelRenderer::ObjectDataPtr&& object) {
	size_t index = (((size_t) object._data) - ((size_t) &_objects.front())) / sizeof(ModelRenderer::ObjectData);

	if (index >= _objects.size()) {
		throw std::invalid_argument("index of ObjectData out of range");
	}

	_objects.erase(_objects.begin() + index);
}

void CustomShaderModelRenderer::RenderToShadowMap() const {
	_vao.Bind();

	_object_data_buffer.SetData(_objects, GLBuffer::STREAM_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, _objects.size());

	GL::ClearVertexArrayBinding();
}

void CustomShaderModelRenderer::RenderObjects() const {
	_shader.Use();
	_vao.Bind();

	_object_data_buffer.SetData(_objects, GLBuffer::STREAM_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, _objects.size());

	GL::ClearVertexArrayBinding();
	GLShaderProgram::ClearUse();
}

GLShaderProgram& CustomShaderModelRenderer::GetShaderProgram() {
	return _shader;
}

GLShaderProgram& CustomShaderModelRenderer::_GetCompiledShader(const Shader& shader) {
	auto address = shader.GetAddress();
	auto iter = _shader_cache.find(address);

	if (iter == _shader_cache.end()) {
		std::string shaderSource = EngineResources::PreprocessShader("Shaders_modelshader_custom_header_frag_glsl");
		shaderSource += "\n\n";
		shaderSource += "#line 1\n";
		shaderSource += shader.GetSource();

		GLShaderProgram shaderProgram;
		shaderProgram.AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_modelshader_vert_glsl"));
		shaderProgram.AddShaderFromSource(GLShaderProgram::FRAGMENT, std::move(shaderSource));
		shaderProgram.Link();

		iter = _shader_cache.emplace(shader.GetAddress(), std::move(shaderProgram)).first;
	}

	return iter->second;
}

}