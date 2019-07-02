#ifndef MODELRENDERER_H_
#define MODELRENDERER_H_
#include "../_common.h"

#include "../Model.h"

#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API ModelRenderer {

public:
	class ObjectData {
		friend class ModelRenderer;

	public:
		void SetTransform(vec3 position, quat rotation, vec3 scale);

	private:
		ObjectData(unsigned index);

		mat4 _model_matrix;
		mat4 _normal_model_matrix;

		unsigned _index;

	};

public:
	ModelRenderer(ModelPtr model);

	ObjectData *AddObject();
	void RemoveObject(ObjectData *object);

	void RenderObjects() const;

private:
public:
	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;
	mutable GLBuffer _object_data_buffer;

	unsigned _index_count;

	std::vector<ObjectData> _objects;

public:
	static GLShaderProgram& GetModelShader();

private:
	static void _InitializeShader();

	static GLShaderProgram _model_shader;
	static bool _is_model_shader_initialized;

};

}

#endif
