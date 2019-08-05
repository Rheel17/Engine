#ifndef MODELRENDERER_H_
#define MODELRENDERER_H_
#include "../_common.h"

#include <set>

#include "../Material.h"
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
		void SetMaterialVector(vec4 materialVector);
		void SetMaterialColor(vec4 materialColor);

	private:
		ObjectData();

		mat4 _model_matrix;
		mat4 _normal_model_matrix;
		vec4 _material_vector;
		vec4 _material_color;

	};

private:
	class _ObjectDataList {

	public:
		_ObjectDataList() = default;
		~_ObjectDataList();

		ObjectData *Add();
		void Remove(ObjectData *data);

		std::vector<ObjectData> Data() const;

	private:
		std::set<ObjectData *> _objects;

	};

	struct _MaterialTextureCompare {
		bool operator()(const Material& mat1, const Material& mat2) const;
	};

public:
	ModelRenderer(ModelPtr model);

	ObjectData *AddObject();
	ObjectData *AddTexturedObject(const Material& material);

	void RemoveObject(ObjectData *object);
	void RemoveTexturedObject(const Material& material, ObjectData *object);

	void RenderObjects() const;

public:
	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;
	mutable GLBuffer _object_data_buffer;

	unsigned _index_count;

	_ObjectDataList _objects;
	std::map<Material, _ObjectDataList, _MaterialTextureCompare> _textured_objects;

public:
	static GLShaderProgram& GetModelShader();

private:
	static void _InitializeShader();

	static GLShaderProgram _model_shader;
	static bool _is_model_shader_initialized;

};

}

#endif
