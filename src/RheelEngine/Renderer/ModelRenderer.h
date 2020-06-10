/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MODELRENDERER_H
#define RHEELENGINE_MODELRENDERER_H
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

		ObjectDataPtr* _ptr;

	};

	class ObjectDataPtr {
		friend class ObjectData;
		friend class ModelRenderer;
		friend class CustomShaderModelRenderer;

		RE_NO_COPY(ObjectDataPtr);

	public:
		ObjectDataPtr() = default;
		explicit ObjectDataPtr(ObjectData* data);

		void SetMatrix(mat4 matrix);
		void SetMaterialVector(vec4 materialVector);
		void SetMaterialColor(vec4 materialColor);

		explicit operator bool() const;
		ObjectDataPtr& operator=(ObjectDataPtr&& ptr) noexcept;

	private:
		ObjectData* _data = nullptr;

	};

private:
	using ObjectDataVector = std::vector<ObjectData>;

	struct material_texture_compare {
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
	static ObjectDataPtr Add_(ObjectDataVector& objects);
	static void Remove_(ObjectDataVector& objects, ObjectDataPtr&& data);

	gl::VertexArray _vao;
	gl::Buffer _vertex_buffer_object;
	mutable gl::Buffer _object_data_buffer;

	ObjectDataVector _objects;
	std::map<Material, ObjectDataVector, material_texture_compare> _textured_objects;

};

}

#endif
