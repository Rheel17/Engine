#ifndef MODELRENDERER_H_
#define MODELRENDERER_H_
#include "../_common.h"

#include "../Material.h"
#include "../Model.h"

#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API ModelRenderer {

public:
	class ObjectDataPtr;

	class ObjectData {
		friend class ModelRenderer;
		friend class ObjectDataPtr;

	public:
		ObjectData();
		ObjectData(const ObjectData& data);
		ObjectData& operator=(ObjectData&& data);

		mat4 _model_matrix;
		mat4 _normal_model_matrix;
		vec4 _material_vector;
		vec4 _material_color;

		ObjectDataPtr *_ptr;
	};

	class ObjectDataPtr {
		friend class ObjectData;
		friend class ModelRenderer;

		RE_NO_COPY(ObjectDataPtr)

	public:
		ObjectDataPtr();
		ObjectDataPtr(ObjectData *data);

		void SetTransform(vec3 position, quat rotation, vec3 scale);
		void SetMaterialVector(vec4 materialVector);
		void SetMaterialColor(vec4 materialColor);

		operator bool() const;
		ObjectDataPtr& operator=(ObjectDataPtr&& ptr);

	private:
		ObjectData *_data;

	};

private:
	using _ObjectDataVector = std::vector<ObjectData>;

	struct _MaterialTextureCompare {
		bool operator()(const Material& mat1, const Material& mat2) const;
	};

public:
	ModelRenderer(ModelPtr model);

	ObjectDataPtr AddObject();
	ObjectDataPtr AddTexturedObject(const Material& material);

	void RemoveObject(ObjectDataPtr&& object);
	void RemoveTexturedObject(const Material& material, ObjectDataPtr&& object);

	void RenderObjects() const;

private:
	ObjectDataPtr _Add(_ObjectDataVector& objects);
	void _Remove(_ObjectDataVector& objects, ObjectDataPtr&& data);

	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;
	mutable GLBuffer _object_data_buffer;

	unsigned _index_count;

	_ObjectDataVector _objects;
	std::map<Material, _ObjectDataVector, _MaterialTextureCompare> _textured_objects;

public:
	static GLShaderProgram& GetModelShader();
	static GLShaderProgram& GetOpaqueShader();

private:
	static void _InitializeShaders();

	static GLShaderProgram _model_shader;
	static GLShaderProgram _opaque_shader;
	static bool _are_shaders_initialized;

};

}

#endif
