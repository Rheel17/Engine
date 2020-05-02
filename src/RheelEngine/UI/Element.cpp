/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Element.h"

#include <utility>

#include "UI.h"
#include "../EngineResources.h"
#include "../Renderer/ImageTexture.h"

namespace rheel {

#define MODE_COLORED    1
#define MODE_TEXTURED   2

Element::ogl_data::ogl_data() :
		ui_vertex_data(gl::Buffer::Target::ARRAY) {

	ui_shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_uishader_vert_glsl"));
	ui_shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_uishader_frag_glsl"));
	ui_shader.Link();

	ivec2 screenDimension = DisplayConfiguration::Get().resolution;
	ui_shader.GetUniform("_screen_dimensions") = vec2{ screenDimension.x, screenDimension.y };
	ui_shader.GetUniform("_texture_sampler") = 0;

	ui_vertex_data.SetDataEmpty(gl::Buffer::Usage::STREAM_DRAW);
	ui_vao.SetVertexAttributes<vec2, vec4, vec2>(ui_vertex_data);

}

bool Element::Bounds::operator==(const Bounds& bounds) const {
	return bounds.x == x && bounds.y == y && bounds.width == width && bounds.height == height;
}

bool Element::Bounds::operator!=(const Bounds& bounds) const {
	return !(*this == bounds);
}

Element::Element() :
		_parent_container(nullptr) {}

void Element::SetDefaultSize(unsigned width, unsigned height) {
	_default_width = width;
	_default_height = height;
}

void Element::SetFocusable(bool focusable) {
	_focusable = focusable;
}

bool Element::IsFocusable() const {
	return _focusable;
}

bool Element::HasFocus() const {
	return RootContainer()->ParentUI()->GetFocusElement() == this;
}

bool Element::RequestFocus() {
	return RootContainer()->ParentUI()->RequestFocus(this);
}

bool Element::LoseFocus() {
	if (HasFocus()) {
		RootContainer()->ParentUI()->RequestFocus(nullptr);
		return true;
	}

	return false;
}

bool Element::IsOpaque() {
	return true;
}

void Element::SetDragEnabled(bool drag) {
	_drag_enabled = drag;
}

bool Element::IsDragEnabled() const {
	return _drag_enabled;
}

void Element::SetBounds(Element::Bounds bounds) {
	_has_initialized_bounds = true;

	if (_bounds != bounds) {
		_bounds = bounds;
		OnResize();
	}
}

const Element::Bounds& Element::GetBounds() const {
	return _bounds;
}

Element::Bounds& Element::GetBounds() {
	return _bounds;
}

void Element::InitializeBounds() {
	if (!_has_initialized_bounds) {
		_bounds.width = _default_width;
		_bounds.height = _default_height;
		_bounds.x = 0;
		_bounds.y = 0;
		_has_initialized_bounds = true;
	}
}

const Container* Element::RootContainer() const {
	if (_parent_container) {
		return _parent_container->RootContainer();
	}

	if (auto container = dynamic_cast<const Container*>(this)) {
		return container;
	}

	return nullptr;
}

Container* Element::RootContainer() {
	if (_parent_container) {
		return _parent_container->RootContainer();
	}

	if (auto container = dynamic_cast<Container*>(this)) {
		return container;
	}

	return nullptr;
}

void Element::AddInputCallback(std::shared_ptr<InputCallback> callback) {
	_callback_list.push_back(std::move(callback));
}

void Element::RemoveInputCallback(const std::shared_ptr<InputCallback>& callback) {
	_callback_list.erase(std::find(_callback_list.begin(), _callback_list.end(), callback));
}

void Element::MoveSuperFields(Element&& element) {
	_parent_container = element._parent_container;
	_bounds = element._bounds;
	_has_initialized_bounds = element._has_initialized_bounds;
	_default_width = element._default_width;
	_default_height = element._default_height;
}

void Element::Callback_(const std::function<void(const CbPtr&)>& callback) {
	for (const CbPtr& ptr : _callback_list) {
		callback(ptr);
	}
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-virtual-near-miss"
void Element::OnResize_() {
	OnResize();
	Callback_([](const CbPtr& ptr) { ptr->OnResize(); });
}

void Element::OnFocusGained_() {
	OnFocusGained();
	Callback_([](const CbPtr& ptr) { ptr->OnFocusGained(); });
}

void Element::OnFocusLost_() {
	OnFocusLost();
	Callback_([](const CbPtr& ptr) { ptr->OnFocusLost(); });
}

void Element::OnKeyPress_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyPress(key, scancode, mods);
	Callback_([key, scancode, mods](const CbPtr& ptr) { ptr->OnKeyPress(key, scancode, mods); });
}

void Element::OnKeyRepeat_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyRepeat(key, scancode, mods);
	Callback_([key, scancode, mods](const CbPtr& ptr) { ptr->OnKeyRepeat(key, scancode, mods); });
}

void Element::OnKeyRelease_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyRelease(key, scancode, mods);
	Callback_([key, scancode, mods](const CbPtr& ptr) { ptr->OnKeyRelease(key, scancode, mods); });
}

void Element::OnCharacterInput_(Input::Unicode character) {
	OnCharacterInput(character);
	Callback_([character](const CbPtr& ptr) { ptr->OnCharacterInput(character); });
}

void Element::OnMouseButtonPress_(Input::MouseButton button, Input::Modifiers mods) {
	OnMouseButtonPress(button, mods);
	Callback_([button, mods](const CbPtr& ptr) { ptr->OnMouseButtonPress(button, mods); });
}

void Element::OnMouseButtonRelease_(Input::MouseButton button, Input::Modifiers mods) {
	OnMouseButtonRelease(button, mods);
	Callback_([button, mods](const CbPtr& ptr) { ptr->OnMouseButtonRelease(button, mods); });
}

void Element::OnMouseEnter_(const vec2& position) {
	OnMouseEnter(position);
	Callback_([position](const CbPtr& ptr) { ptr->OnMouseEnter(position); });
}

void Element::OnMouseExit_(const vec2& position) {
	OnMouseExit(position);
	Callback_([position](const CbPtr& ptr) { ptr->OnMouseExit(position); });
}

void Element::OnMouseMove_(const vec2& position) {
	OnMouseMove(position);
	Callback_([position](const CbPtr& ptr) { ptr->OnMouseMove(position); });
}

void Element::OnMouseJump_(const vec2& position) {
	OnMouseJump(position);
	Callback_([position](const CbPtr& ptr) { ptr->OnMouseJump(position); });
}

void Element::OnMouseDrag_(const vec2& origin, const vec2& position) {
	OnMouseDrag(origin, position);
	Callback_([origin, position](const CbPtr& ptr) { ptr->OnMouseDrag(origin, position); });
}

void Element::OnMouseScroll_(const vec2& scrollComponents) {
	OnMouseScroll(scrollComponents);
	Callback_([scrollComponents](const CbPtr& ptr) { ptr->OnMouseScroll(scrollComponents); });
}
#pragma clang diagnostic pop

const TextRenderer& Element::GetTextRenderer() const {
	return _ogl_data->text_renderer;
}

void Element::DrawColoredTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) const {
	Draw_({ v1, v2, v3 }, MODE_COLORED);
}

void Element::DrawColoredQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) const {
	Draw_({ v1, v2, v3, v3, v4, v1 }, MODE_COLORED);
}

void Element::DrawColoredQuad(const Bounds& bounds, const Color& color) const {
	DrawColoredQuad(
			Vertex({ bounds.x, bounds.y }, color),
			Vertex({ bounds.x, bounds.y + bounds.height }, color),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, color),
			Vertex({ bounds.x + bounds.width, bounds.y }, color));
}

void Element::DrawTexturedTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const gl::Texture2D& texture) const {
	texture.Bind(0);
	Draw_({ v1, v2, v3 }, MODE_TEXTURED);
}

void Element::DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const gl::Texture2D& texture) const {
	texture.Bind(0);
	Draw_({ v1, v2, v3, v3, v4, v1 }, MODE_TEXTURED);
}

void Element::DrawTexturedQuad(const Bounds& bounds, const gl::Texture2D& texture) const {
	DrawTexturedQuad(
			Vertex({ bounds.x, bounds.y }, { 0.0f, 1.0f }),
			Vertex({ bounds.x, bounds.y + bounds.height }, { 0.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, { 1.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y }, { 1.0f, 1.0f }),
			texture);
}

void Element::DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const Image& image, float alpha) const {
	ImageTexture::Get(image).Bind(0);
	Draw_({ v1, v2, v3, v3, v4, v1 }, MODE_TEXTURED, alpha);
}

void Element::DrawTexturedQuad(const Bounds& bounds, const Image& image, float alpha) const {
	DrawTexturedQuad(
			Vertex({ bounds.x, bounds.y }, { 0.0f, 1.0f }),
			Vertex({ bounds.x, bounds.y + bounds.height }, { 0.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, { 1.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y }, { 1.0f, 1.0f }),
			image, alpha);
}

void Element::DrawShaderedQuad(const Bounds& bounds, const Shader& shader) const {
	Vertex v1({ bounds.x, bounds.y });
	Vertex v2({ bounds.x, bounds.y + bounds.height });
	Vertex v3({ bounds.x + bounds.width, bounds.y + bounds.height });
	Vertex v4({ bounds.x + bounds.width, bounds.y });

	const gl::Program& program = GetCustomShader(shader);
	program.Use();

	std::vector<Vertex> vertices = { v1, v2, v3, v3, v4, v1 };
	_ogl_data->ui_vertex_data.SetData(vertices, gl::Buffer::Usage::STREAM_DRAW);
	_ogl_data->ui_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, vertices.size());
}

const gl::Program& Element::GetCustomShader(const Shader& shader) const {
	auto address = shader.GetAddress();

	return _ogl_data->custom_shaders.Get(address, [shader](std::uintptr_t){
		std::string shaderSource = EngineResources::PreprocessShader("Shaders_uishader_custom_header_frag_glsl");
		shaderSource += "\n\n";
		shaderSource += "#line 1\n";
		shaderSource += shader.GetSource();

		gl::Program shaderProgram;
		shaderProgram.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_uishader_vert_glsl"));
		shaderProgram.AttachShader(gl::Shader::ShaderType::FRAGMENT, shaderSource);
		shaderProgram.Link();

		ivec2 screenDimension = DisplayConfiguration::Get().resolution;
		shaderProgram["_screen_dimensions"] = vec2{ screenDimension.x, screenDimension.y };

		return shaderProgram;
	});
}

void Element::Draw_(const std::vector<Vertex>& vertices, int mode, float alpha) const {
	if (mode == MODE_COLORED) {
		gl::Context::Current().ClearTexture(0, gl::Texture::Target::TEXTURE_2D);
	}

	_ogl_data->ui_shader.GetUniform("_ui_mode") = mode;
	_ogl_data->ui_shader.GetUniform("_alpha") = alpha;
	_ogl_data->ui_vertex_data.SetData(vertices, gl::Buffer::Usage::STREAM_DRAW);
	_ogl_data->ui_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, vertices.size());
}

}
