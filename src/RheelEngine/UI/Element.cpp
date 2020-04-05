/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Element.h"

#include <utility>

#include "../Engine.h"
#include "../EngineResources.h"
#include "../Renderer/ImageTexture.h"

namespace rheel {

#define MODE_COLORED    1
#define MODE_TEXTURED   2

std::unique_ptr<GL::Program> Element::_ui_shader(nullptr);
std::unique_ptr<GL::VertexArray> Element::_ui_vao(nullptr);
std::unique_ptr<GL::Buffer> Element::_ui_vertex_data(nullptr);
bool Element::_initialized = false;

std::unordered_map<std::uintptr_t, GL::Program> Element::_custom_shaders;

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
	return RootContainer()->ParentUI()->FocusElement() == this;
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


const Container *Element::RootContainer() const {
	if (_parent_container) {
		return _parent_container->RootContainer();
	}

	if (auto container = dynamic_cast<const Container *>(this)) {
		return container;
	}

	return nullptr;
}

Container *Element::RootContainer() {
	if (_parent_container) {
		return _parent_container->RootContainer();
	}

	if (auto container = dynamic_cast<Container *>(this)) {
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

void Element::_MoveSuperFields(Element&& element) {
	_parent_container = element._parent_container;
	_bounds = element._bounds;
	_has_initialized_bounds = element._has_initialized_bounds;
	_default_width = element._default_width;
	_default_height = element._default_height;
}

void Element::_Callback(const std::function<void(const _CBPtr&)>& callback) {
	for (const _CBPtr& ptr : _callback_list) {
		callback(ptr);
	}
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-virtual-near-miss"
void Element::_OnResize() {
	OnResize();
	_Callback([](const _CBPtr& ptr) { ptr->OnResize(); });
}

void Element::_OnFocusGained() {
	OnFocusGained();
	_Callback([](const _CBPtr& ptr) { ptr->OnFocusGained(); });
}

void Element::_OnFocusLost() {
	OnFocusLost();
	_Callback([](const _CBPtr& ptr) { ptr->OnFocusLost(); });
}

void Element::_OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyPress(key, scancode, mods);
	_Callback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyPress(key, scancode, mods); });
}

void Element::_OnKeyRepeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyRepeat(key, scancode, mods);
	_Callback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyRepeat(key, scancode, mods); });
}

void Element::_OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	 OnKeyRelease(key, scancode, mods);
	 _Callback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyRelease(key, scancode, mods); });
}

void Element::_OnCharacterInput(Input::Unicode character) {
	OnCharacterInput(character);
	_Callback([character](const _CBPtr& ptr) { ptr->OnCharacterInput(character); });
}

void Element::_OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	OnMouseButtonPress(button, mods);
	_Callback([button, mods](const _CBPtr& ptr) { ptr->OnMouseButtonPress(button, mods); });
}

void Element::_OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	OnMouseButtonRelease(button, mods);
	_Callback([button, mods](const _CBPtr& ptr) { ptr->OnMouseButtonRelease(button, mods); });
}

void Element::_OnMouseEnter(const vec2& position) {
	OnMouseEnter(position);
	_Callback([position](const _CBPtr& ptr) { ptr->OnMouseEnter(position); });
}

void Element::_OnMouseExit(const vec2& position) {
	OnMouseExit(position);
	_Callback([position](const _CBPtr& ptr) { ptr->OnMouseExit(position); });
}

void Element::_OnMouseMove(const vec2& position) {
	OnMouseMove(position);
	_Callback([position](const _CBPtr& ptr) { ptr->OnMouseMove(position); });
}

void Element::_OnMouseJump(const vec2& position) {
	OnMouseJump(position);
	_Callback([position](const _CBPtr& ptr) { ptr->OnMouseJump(position); });
}

void Element::_OnMouseDrag(const vec2& origin, const vec2& position) {
	OnMouseDrag(origin, position);
	_Callback([origin, position](const _CBPtr& ptr) { ptr->OnMouseDrag(origin, position); });
}

void Element::_OnMouseScroll(const vec2& scrollComponents) {
	OnMouseScroll(scrollComponents);
	_Callback([scrollComponents](const _CBPtr& ptr) { ptr->OnMouseScroll(scrollComponents); });
}
#pragma clang diagnostic pop

void Element::_DrawColoredTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	_Draw({ v1, v2, v3 }, MODE_COLORED);
}

void Element::_DrawColoredQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) {
	_Draw({ v1, v2, v3, v3, v4, v1 }, MODE_COLORED);
}

void Element::_DrawColoredQuad(const Bounds& bounds, const Color& color) {
	_DrawColoredQuad(
			Vertex({ bounds.x, bounds.y }, color),
			Vertex({ bounds.x, bounds.y + bounds.height }, color),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, color),
			Vertex({ bounds.x + bounds.width, bounds.y }, color));
}

void Element::_DrawTexturedTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const GL::Texture2D& texture) {
	texture.Bind(0);
	_Draw({ v1, v2, v3 }, MODE_TEXTURED);
}

void Element::_DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const GL::Texture2D& texture) {
	texture.Bind(0);
	_Draw({ v1, v2, v3, v3, v4, v1 }, MODE_TEXTURED);
}

void Element::_DrawTexturedQuad(const Bounds& bounds, const GL::Texture2D& texture) {
	_DrawTexturedQuad(
			Vertex({ bounds.x, bounds.y }, { 0.0f, 1.0f }),
			Vertex({ bounds.x, bounds.y + bounds.height }, { 0.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, { 1.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y }, { 1.0f, 1.0f }),
			texture);
}

void Element::_DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const Image& image, float alpha) {
	ImageTexture::Get(image).Bind(0);
	_Draw({ v1, v2, v3, v3, v4, v1 }, MODE_TEXTURED, alpha);
}

void Element::_DrawTexturedQuad(const Bounds& bounds, const Image& image, float alpha) {
	_DrawTexturedQuad(
			Vertex({ bounds.x, bounds.y }, { 0.0f, 1.0f }),
			Vertex({ bounds.x, bounds.y + bounds.height }, { 0.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, { 1.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y }, { 1.0f, 1.0f }),
			image, alpha);
}

void Element::_DrawShaderedQuad(const Element::Bounds& bounds, const Shader& shader) {
	_Initialize();

	Vertex v1({ bounds.x, bounds.y });
	Vertex v2({ bounds.x, bounds.y + bounds.height });
	Vertex v3({ bounds.x + bounds.width, bounds.y + bounds.height });
	Vertex v4({ bounds.x + bounds.width, bounds.y });

	const GL::Program& program = _GetCustomShader(shader);
	program.Use();

	std::vector<Vertex> vertices = { v1, v2, v3, v3, v4, v1 };
	_ui_vertex_data->SetData(vertices, GL::Buffer::Usage::STREAM_DRAW);
	_ui_vao->DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, vertices.size());
}

const GL::Program& Element::_GetCustomShader(const Shader& shader) {
	auto address = shader.GetAddress();
	auto iter = _custom_shaders.find(address);

	if (iter == _custom_shaders.end()) {
		std::string shaderSource = EngineResources::PreprocessShader("Shaders_uishader_custom_header_frag_glsl");
		shaderSource += "\n\n";
		shaderSource += "#line 1\n";
		shaderSource += shader.GetSource();

		GL::Program shaderProgram;
		shaderProgram.AttachShader(GL::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_uishader_vert_glsl"));
		shaderProgram.AttachShader(GL::Shader::ShaderType::FRAGMENT, shaderSource);
		shaderProgram.Link();

		const DisplayConfiguration::Resolution& screenDimension = Engine::GetDisplayConfiguration().resolution;
		shaderProgram["_screen_dimensions"] = vec2 { screenDimension.width, screenDimension.height };

		iter = _custom_shaders.emplace(shader.GetAddress(), std::move(shaderProgram)).first;
	}

	return iter->second;
}

void Element::_Draw(const std::vector<Vertex>& vertices, int mode, float alpha) {
	_Initialize();

	_ui_shader->GetUniform("_ui_mode") = mode;
	_ui_shader->GetUniform("_alpha") = alpha;
	_ui_vertex_data->SetData(vertices, GL::Buffer::Usage::STREAM_DRAW);
	_ui_vao->DrawArrays(GL::VertexArray::Mode::TRIANGLES, 0, vertices.size());
}

void Element::_Initialize() {
	if (_initialized) {
		return;
	}

	_ui_shader = std::make_unique<GL::Program>();
	_ui_shader->AttachShader(GL::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_uishader_vert_glsl"));
	_ui_shader->AttachShader(GL::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_uishader_frag_glsl"));
	_ui_shader->Link();

	const DisplayConfiguration::Resolution& screenDimension = Engine::GetDisplayConfiguration().resolution;
	_ui_shader->GetUniform("_screen_dimensions") = vec2 { screenDimension.width, screenDimension.height };
	_ui_shader->GetUniform("_texture_sampler") = 0;

	_ui_vertex_data = std::make_unique<GL::Buffer>(GL::Buffer::Target::ARRAY);
	_ui_vao = std::make_unique<GL::VertexArray>();
	_ui_vao->SetVertexAttributes<vec2, vec4, vec2>(*_ui_vertex_data);

	_initialized = true;
}

}
