///*
// * Copyright © 2019 Levi van Rheenen. All rights reserved.
// */
//#include "ModelRenderer.h"
//
//#include "../EngineResources.h"
//
//#include <array>
//
//namespace rheel {
//
//GLShaderProgram ModelRenderer::_forward_model_shader;
//GLShaderProgram ModelRenderer::_deferred_model_shader;
//GLShaderProgram ModelRenderer::_opaque_shader;
//bool ModelRenderer::_are_shaders_initialized = false;
//
//ModelRenderer::ObjectData::ObjectData() :
//		_ptr(nullptr) {}
//
//ModelRenderer::ObjectData::ObjectData(const ObjectData& data) :
//		_model_matrix(data._model_matrix),
//		_normal_model_matrix(data._normal_model_matrix),
//		_material_vector(data._material_vector),
//		_material_color(data._material_color),
//		_ptr(data._ptr) {
//
//	if (_ptr) {
//		_ptr->_data = this;
//	}
//}
//
//ModelRenderer::ObjectData& ModelRenderer::ObjectData::operator=(ObjectData&& data) {
//	_model_matrix = std::move(data._model_matrix);
//	_normal_model_matrix = std::move(data._normal_model_matrix);
//	_material_vector = std::move(data._material_vector);
//	_material_color = std::move(data._material_color);
//	_ptr = data._ptr;
//
//	if (_ptr) {
//		_ptr->_data = this;
//	}
//
//	return *this;
//}
//
//ModelRenderer::ObjectDataPtr::ObjectDataPtr(ObjectData *data) :
//		_data(data) {
//
//	_data->_ptr = this;
//}
//
//void ModelRenderer::ObjectDataPtr::SetTransform(mat4 matrix) {
//	_data->_model_matrix = matrix;
//	_data->_normal_model_matrix = glm::transpose(glm::inverse(mat3(_data->_model_matrix)));
//}
//
//void ModelRenderer::ObjectDataPtr::SetMaterialVector(vec4 materialVector) {
//	_data->_material_vector = std::move(materialVector);
//}
//
//void ModelRenderer::ObjectDataPtr::SetMaterialColor(vec4 materialColor) {
//	_data->_material_color = std::move(materialColor);
//}
//
//ModelRenderer::ObjectDataPtr::operator bool() const {
//	return _data;
//}
//
//ModelRenderer::ObjectDataPtr& ModelRenderer::ObjectDataPtr::operator=(ObjectDataPtr&& ptr) {
//	_data = std::move(ptr._data);
//	_data->_ptr = this;
//	return *this;
//}
//
//bool ModelRenderer::_MaterialTextureCompare::operator()(const Material& mat1, const Material& mat2) const {
//	std::array<std::uintptr_t, 3> mat1Addresses = {{
//			std::uintptr_t(mat1.AmbientTexture()),
//			std::uintptr_t(mat1.DiffuseTexture()),
//			std::uintptr_t(mat1.SpecularTexture()) }};
//	std::array<std::uintptr_t, 3> mat2Addresses = {{
//			std::uintptr_t(mat2.AmbientTexture()),
//			std::uintptr_t(mat2.DiffuseTexture()),
//			std::uintptr_t(mat2.SpecularTexture()) }};
//
//	return std::tie(mat1Addresses[0], mat1Addresses[1], mat1Addresses[2]) <
//			std::tie(mat2Addresses[0], mat2Addresses[1], mat2Addresses[2]);
//}
//
//ModelRenderer::ModelRenderer(const Model& model) :
//		_vertex_buffer_object(GL::BufferTarget::ARRAY),
//		_element_array_buffer(GL::BufferTarget::ELEMENT_ARRAY),
//		_object_data_buffer(GL::BufferTarget::ARRAY),
//		_index_count(model.Indices().size()) {
//
//	_vertex_buffer_object.SetData(model.Vertices());
//	_element_array_buffer.SetData(model.Indices());
//	_object_data_buffer.SetData(std::vector<ObjectData>());
//
//	_vao.SetVertexAttributes<vec3, vec3, vec2>(_vertex_buffer_object);
//	_vao.SetVertexIndices(_element_array_buffer);
//	_vao.SetVertexAttributes<mat4, mat4, vec4, vec4>(_object_data_buffer, sizeof(ObjectData), true);
//
//	GL::ClearVertexArrayBinding();
//}
//
//ModelRenderer::ObjectDataPtr ModelRenderer::AddObject() {
//	return _Add(_objects);
//}
//
//ModelRenderer::ObjectDataPtr ModelRenderer::AddTexturedObject(const Material& material) {
//	return _Add(_textured_objects[material]);
//}
//
//void ModelRenderer::RemoveObject(ObjectDataPtr&& object) {
//	_Remove(_objects, std::move(object));
//}
//
//void ModelRenderer::RemoveTexturedObject(const Material& material, ObjectDataPtr&& object) {
//	_Remove(_textured_objects[material], std::move(object));
//}
//
//GLShaderProgram& ModelRenderer::GetForwardModelShader() {
//	_InitializeShaders();
//	return _forward_model_shader;
//}
//
//GLShaderProgram& ModelRenderer::GetDeferredModelShader() {
//	_InitializeShaders();
//	return _deferred_model_shader;
//}
//
//GLShaderProgram& ModelRenderer::GetOpaqueShader() {
//	_InitializeShaders();
//	return _opaque_shader;
//}
//
//void ModelRenderer::RenderObjects() const {
//	_vao.Bind();
//
//	GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, 0);
//	GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, 1);
//	GL::ClearTextureBinding(GL::TextureTarget::TEXTURE_2D, 2);
//
//	_object_data_buffer.SetData(_objects, GLBuffer::STREAM_DRAW);
//	glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, _objects.size());
//
//	for (auto pair : _textured_objects) {
//		pair.first.BindTextures();
//
//		_object_data_buffer.SetData(pair.second, GLBuffer::STREAM_DRAW);
//		glDrawElementsInstanced(GL_TRIANGLES, _index_count, GL_UNSIGNED_INT, nullptr, pair.second.size());
//	}
//
//	GL::ClearVertexArrayBinding();
//}
//
//ModelRenderer::ObjectDataPtr ModelRenderer::_Add(_ObjectDataVector& objects) {
//	return &objects.emplace_back();
//}
//
//void ModelRenderer::_Remove(_ObjectDataVector& objects, ObjectDataPtr&& data) {
//	size_t index = (((size_t) data._data) - ((size_t) &objects.front())) / sizeof(ObjectData);
//
//	if (index >= objects.size()) {
//		throw std::invalid_argument("index of ObjectData out of range");
//	}
//
//	objects.erase(objects.begin() + index);
//}
//
//void ModelRenderer::_InitializeShaders() {
//	if (_are_shaders_initialized) {
//		return;
//	}
//
//	_forward_model_shader.AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_forward_modelshader_vert_glsl"));
//	_forward_model_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_forward_modelshader_frag_glsl"));
//	_forward_model_shader.Link();
//	_forward_model_shader["ambientTexture"] = 0;
//	_forward_model_shader["diffuseTexture"] = 1;
//	_forward_model_shader["specularTexture"] = 2;
//	_forward_model_shader["shadowMap0"] = 3;
//	_forward_model_shader["shadowMap1"] = 4;
//	_forward_model_shader["shadowMap2"] = 5;
//	_forward_model_shader["shadowMap3"] = 6;
//
//	_deferred_model_shader.AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_deferred_modelshader_vert_glsl"));
//	_deferred_model_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_deferred_modelshader_frag_glsl"));
//	_deferred_model_shader.Link();
//	_deferred_model_shader["ambientTexture"] = 0;
//	_deferred_model_shader["diffuseTexture"] = 1;
//	_deferred_model_shader["specularTexture"] = 2;
//
//	_opaque_shader.AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_opaqueshader_vert_glsl"));
//	_opaque_shader.AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_opaqueshader_frag_glsl"));
//	_opaque_shader.Link();
//
//	_are_shaders_initialized = true;
//}
//
//}
