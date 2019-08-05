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
	class ObjectData {
		friend class ModelRenderer;

		// Make sure that our vectors can copy instances. This is probably not
		// the correct way of doing this, but it works for now.
		template<typename _T1, typename... _Args>
		friend void std::_Construct(_T1* __p, _Args&&... __args);
		friend class std::vector<ObjectData>;

	public:
		ObjectData();

		void SetTransform(vec3 position, quat rotation, vec3 scale);
		void SetMaterialVector(vec4 materialVector);
		void SetMaterialColor(vec4 materialColor);

	private:
		ObjectData(const ObjectData& data);

		mat4 _model_matrix;
		mat4 _normal_model_matrix;
		vec4 _material_vector;
		vec4 _material_color;

	public:
		// fields we won't use on the GPU go after the ones that we do.
		ObjectData **change_ptr = nullptr;

	};

private:
	using _ObjectDataVector = std::vector<ObjectData>;

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

private:
	ObjectData *_Add(_ObjectDataVector& objects);
	void _Remove(_ObjectDataVector& objects, ObjectData *data);

	GLVertexArray _vao;
	GLBuffer _vertex_buffer_object;
	GLBuffer _element_array_buffer;
	mutable GLBuffer _object_data_buffer;

	unsigned _index_count;

	_ObjectDataVector _objects;
	std::map<Material, _ObjectDataVector, _MaterialTextureCompare> _textured_objects;

public:
	static GLShaderProgram& GetModelShader();

private:
	static void _InitializeShader();

	static GLShaderProgram _model_shader;
	static bool _is_model_shader_initialized;

};

}

#endif
