/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CUSTOMSHADERMODELRENDERER_H
#define RHEELENGINE_CUSTOMSHADERMODELRENDERER_H
#include "../_common.h"

#include "ModelRenderer.h"
#include "../Assets/Shader.h"

namespace rheel {

class RE_API CustomShaderModelRenderer {

public:
	CustomShaderModelRenderer(const Model& model, const Shader& shader);

	ModelRenderer::ObjectDataPtr AddObject();

	void RemoveObject(ModelRenderer::ObjectDataPtr&& object);

	void RenderToShadowMap() const;

	void RenderObjects() const;

	_GLShaderProgram& GetShaderProgram();

private:
	_GLVertexArray _vao;
	_GLBuffer _vertex_buffer_object;
	_GLBuffer _element_array_buffer;
	mutable _GLBuffer _object_data_buffer;
	unsigned _index_count;

	_GLShaderProgram _shader;

	ModelRenderer::_ObjectDataVector _objects;

private:
	static std::unordered_map<std::uintptr_t, _GLShaderProgram> _shader_cache;

	static _GLShaderProgram& _GetCompiledShader(const Shader& shader);

};

}

#endif
