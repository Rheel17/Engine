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

	GL::Program& GetShaderProgram();

private:
	GL::VertexArray _vao;
	GL::Buffer _vertex_buffer_object;
	GL::Buffer _element_array_buffer;
	mutable GL::Buffer _object_data_buffer;
	unsigned _index_count;

	GL::Program _shader;

	ModelRenderer::_ObjectDataVector _objects;

private:
	static std::unordered_map<std::uintptr_t, GL::Program> _shader_cache;

	static GL::Program& _GetCompiledShader(const Shader& shader);

};

}

#endif
