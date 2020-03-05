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

	GLShaderProgram& GetShaderProgram();

private:
	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;
	mutable GLBuffer _object_data_buffer;
	unsigned _index_count;

	GLShaderProgram _shader;

	ModelRenderer::_ObjectDataVector _objects;

private:
	static std::unordered_map<std::uintptr_t, GLShaderProgram> _shader_cache;

	static GLShaderProgram& _GetCompiledShader(const Shader& shader);

};

}

#endif
