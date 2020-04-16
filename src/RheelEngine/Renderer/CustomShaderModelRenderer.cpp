/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "CustomShaderModelRenderer.h"

#include "../EngineResources.h"
#include "OpenGL/State.h"

namespace rheel {

std::unordered_map<std::uintptr_t, gl::Program> CustomShaderModelRenderer::_shader_cache;

CustomShaderModelRenderer::CustomShaderModelRenderer(const Model& model, const Shader& shader) :
		_vertex_buffer_object(gl::Buffer::Target::ARRAY),
		_object_data_buffer(gl::Buffer::Target::ARRAY),
		_shader(std::move(GetCompiledShader_(shader))) {

	_vertex_buffer_object.SetData(model.GetVertices());
	_object_data_buffer.SetData(std::vector<ModelRenderer::ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(model.GetIndices());
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ModelRenderer::ObjectData), true);
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

	_object_data_buffer.SetData(_objects, gl::Buffer::Usage::STREAM_DRAW);
	_vao.DrawElements(gl::VertexArray::Mode::TRIANGLES, _objects.size());
}

void CustomShaderModelRenderer::RenderObjects() const {
	_shader.Use();
	_vao.Bind();

	_object_data_buffer.SetData(_objects, gl::Buffer::Usage::STREAM_DRAW);
	_vao.DrawElements(gl::VertexArray::Mode::TRIANGLES, _objects.size());
}

gl::Program& CustomShaderModelRenderer::GetShaderProgram() {
	return _shader;
}

gl::Program& CustomShaderModelRenderer::GetCompiledShader_(const Shader& shader) {
	auto address = shader.GetAddress();
	auto iter = _shader_cache.find(address);

	if (iter == _shader_cache.end()) {
		std::string shaderSource = EngineResources::PreprocessShader("Shaders_modelshader_custom_header_frag_glsl");
		shaderSource += "\n\n";
		shaderSource += "#line 1\n";
		shaderSource += shader.GetSource();

		gl::Program shaderProgram;
		shaderProgram.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_modelshader_vert_glsl"));
		shaderProgram.AttachShader(gl::Shader::ShaderType::FRAGMENT, shaderSource);
		shaderProgram.Link();

		if (shaderProgram.HasUniform("_shadowMap0")) {
			shaderProgram["_shadowMap0"] = 3;
		}

		if (shaderProgram.HasUniform("_shadowMap1")) {
			shaderProgram["_shadowMap1"] = 4;
		}

		if (shaderProgram.HasUniform("_shadowMap2")) {
			shaderProgram["_shadowMap2"] = 5;
		}

		if (shaderProgram.HasUniform("_shadowMap3")) {
			shaderProgram["_shadowMap3"] = 6;
		}

		iter = _shader_cache.emplace(shader.GetAddress(), std::move(shaderProgram)).first;
	}

	return iter->second;
}

}