/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef MODELRENDERER_H_
#define MODELRENDERER_H_
#include "../_common.h"

#include <map>

#include "../Material.h"
#include "../Assets/Model.h"

#include "OpenGL/Program.h"
#include "OpenGL/VertexArray.h"

namespace rheel {

class RE_API ModelRenderer {
	friend class CustomShaderModelRenderer;

public:
	class ObjectDataPtr;

	class ObjectData {
		friend class ModelRenderer;
		friend class ObjectDataPtr;

	public:
		ObjectData();
		ObjectData(const ObjectData& data);
		ObjectData& operator=(ObjectData&& data) noexcept;

	private:
		mat4 _model_matrix;
		mat4 _normal_model_matrix;
		vec4 _material_vector;
		vec4 _material_color;

		ObjectDataPtr *_ptr;

	};

	class ObjectDataPtr {
		friend class ObjectData;
		friend class ModelRenderer;
		friend class CustomShaderModelRenderer;

		RE_NO_COPY(ObjectDataPtr)

	public:
		ObjectDataPtr() = default;
		explicit ObjectDataPtr(ObjectData *data);

		void SetMatrix(mat4 matrix);
		void SetMaterialVector(vec4 materialVector);
		void SetMaterialColor(vec4 materialColor);

		explicit operator bool() const;
		ObjectDataPtr& operator=(ObjectDataPtr&& ptr) noexcept;

	private:
		ObjectData *_data = nullptr;

	};

private:
	using _ObjectDataVector = std::vector<ObjectData>;

	struct _MaterialTextureCompare {
		bool operator()(const Material& mat1, const Material& mat2) const;
	};

	struct _MaterialShaderCompare {
		bool operator()(const Material& mat1, const Material& mat2) const;
	};

public:
	explicit ModelRenderer(const Model& model);

	ObjectDataPtr AddObject();
	ObjectDataPtr AddTexturedObject(const Material& material);

	void RemoveObject(ObjectDataPtr&& object);
	void RemoveTexturedObject(const Material& material, ObjectDataPtr&& object);

	void RenderObjects() const;

private:
	static ObjectDataPtr _Add(_ObjectDataVector& objects);
	static void _Remove(_ObjectDataVector& objects, ObjectDataPtr&& data);

	GL::VertexArray _vao;
	GL::Buffer _vertex_buffer_object;
	GL::Buffer _element_array_buffer;
	mutable GL::Buffer _object_data_buffer;

	unsigned _index_count;

	_ObjectDataVector _objects;
	std::map<Material, _ObjectDataVector, _MaterialTextureCompare> _textured_objects;

public:
	static GL::Program& GetForwardModelShader();
	static GL::Program& GetOpaqueShader();

private:
	static void _InitializeShaders();

	static GL::Program _forward_model_shader;
	static GL::Program _opaque_shader;
	static bool _are_shaders_initialized;

};

}

#endif
