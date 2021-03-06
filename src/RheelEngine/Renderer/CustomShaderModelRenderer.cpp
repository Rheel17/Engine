/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "CustomShaderModelRenderer.h"

#include "../EngineResources.h"
#include "OpenGL/Context.h"

namespace rheel {

CustomShaderModelRenderer::CustomShaderModelRenderer(const Model& model, const Shader& shader) :
		_vertex_buffer_object(gl::Buffer::Target::ARRAY),
		_object_data_buffer(gl::Buffer::Target::ARRAY),
		_shader(_get_compiled_shader(shader)) {

	_vertex_buffer_object.SetData(model.GetVertices());
	_object_data_buffer.SetData(std::vector<ModelRenderer::ObjectData>());

	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
	_vao.SetVertexIndices(model.GetIndices());
	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ModelRenderer::ObjectData), 1);
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

gl::Program& CustomShaderModelRenderer::_get_compiled_shader(const Shader& shader) {
	auto address = shader.GetAddress();

	return _shader_cache->cache.Get(address, [shader](std::uintptr_t) {
		gl::ContextScope cs;

		std::string shader_source = EngineResources::PreprocessShader("Shaders_modelshader_custom_header_frag_glsl");
		shader_source += "\n\n";
		shader_source += "#line 1\n";
		shader_source += shader.GetSource();

		gl::Program shader_program;
		shader_program.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_modelshader_vert_glsl"));
		shader_program.AttachShader(gl::Shader::ShaderType::FRAGMENT, shader_source);
		shader_program.Link();

		if (shader_program.HasUniform("_shadowMap0")) {
			shader_program["_shadowMap0"] = 3;
		}

		if (shader_program.HasUniform("_shadowMap1")) {
			shader_program["_shadowMap1"] = 4;
		}

		if (shader_program.HasUniform("_shadowMap2")) {
			shader_program["_shadowMap2"] = 5;
		}

		if (shader_program.HasUniform("_shadowMap3")) {
			shader_program["_shadowMap3"] = 6;
		}

		return shader_program;
	});
}

}