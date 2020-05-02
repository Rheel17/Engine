/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CUSTOMSHADERMODELRENDERER_H
#define RHEELENGINE_CUSTOMSHADERMODELRENDERER_H
#include "../_common.h"

#include "ModelRenderer.h"
#include "../Assets/Shader.h"
#include "../Util/Cache.h"

namespace rheel {

class RE_API CustomShaderModelRenderer {

	struct RE_API shader_cache {
		Cache<std::uintptr_t, gl::Program> cache;
	};
	pseudo_static_pointer<shader_cache> _shader_cache;

public:
	CustomShaderModelRenderer(const Model& model, const Shader& shader);

	ModelRenderer::ObjectDataPtr AddObject();

	void RemoveObject(ModelRenderer::ObjectDataPtr&& object);

	void RenderToShadowMap() const;

	void RenderObjects() const;

	gl::Program& GetShaderProgram();

private:
	gl::VertexArray _vao;
	gl::Buffer _vertex_buffer_object;
	mutable gl::Buffer _object_data_buffer;

	gl::Program& _shader;

	ModelRenderer::ObjectDataVector _objects;

private:
	gl::Program& GetCompiledShader_(const Shader& shader);

};

}

#endif
