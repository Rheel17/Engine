#include "Element.h"

#include "../Engine.h"
#include "../EngineResources.h"

namespace rheel {

#define MODE_COLORED    1
#define MODE_TEXTURED   2

std::unique_ptr<GLShaderProgram> Element::_ui_shader(nullptr);
std::unique_ptr<GLVertexArray> Element::_ui_vao(nullptr);
std::unique_ptr<GLBuffer> Element::_ui_vertex_data(nullptr);
bool Element::_initialized = false;

bool Element::Bounds::operator==(const Bounds& bounds) const {
	return bounds.x == x && bounds.y == y && bounds.width == width && bounds.height == height;
}

bool Element::Bounds::operator!=(const Bounds& bounds) const {
	return !(*this == bounds);
}

Element::Vertex::Vertex(vec2 position) :
		_position(std::move(position)), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(0.0f, 0.0f) {}

Element::Vertex::Vertex(vec2 position, vec4 color) :
		_position(std::move(position)), _color(std::move(color)), _texture(0.0f, 0.0f) {}

Element::Vertex::Vertex(vec2 position, vec2 texture) :
		_position(std::move(position)), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(std::move(texture)) {}

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

void Element::SetDragEnabled(bool drag) {
	_drag_enabled = drag;
}

bool Element::IsDragEnabled() const {
	return _drag_enabled;
}

void Element::SetBounds(Element::Bounds bounds) {
	_has_initialized_bounds = true;

	if (_bounds != bounds) {
		_bounds = std::move(bounds);
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

	if (const Container *container = dynamic_cast<const Container *>(this)) {
		return container;
	}

	return nullptr;
}

Container *Element::RootContainer() {
	if (_parent_container) {
		return _parent_container->RootContainer();
	}

	if (Container *container = dynamic_cast<Container *>(this)) {
		return container;
	}

	return nullptr;
}

void Element::AddInputCallback(std::shared_ptr<InputCallback> callback) {
	_callback_list.push_back(std::move(callback));
}

void Element::RemoveInputCallback(std::shared_ptr<InputCallback> callback) {
	_callback_list.erase(std::find(_callback_list.begin(), _callback_list.end(), callback));
}

void Element::_MoveSuperFields(Element&& element) {
	_parent_container = element._parent_container;
	_bounds = std::move(element._bounds);
	_has_initialized_bounds = element._has_initialized_bounds;
	_default_width = element._default_width;
	_default_height = element._default_height;
}

void Element::_VoidCallback(std::function<void(const _CBPtr&)> callback) {
	for (const _CBPtr& ptr : _callback_list) {
		callback(ptr);
	}
}

bool Element::_BoolCallback(std::function<bool(const _CBPtr&)> callback) {
	bool flag = false;

	for (const _CBPtr& ptr : _callback_list) {
		flag |= callback(ptr);
	}

	return flag;
}

void Element::_OnResize() {
	OnResize();
	_VoidCallback([](const _CBPtr& ptr) { ptr->OnResize(); });
}

void Element::_OnFocusGained() {
	OnFocusGained();
	_VoidCallback([](const _CBPtr& ptr) { ptr->OnFocusGained(); });
}

void Element::_OnFocusLost() {
	OnFocusLost();
	_VoidCallback([](const _CBPtr& ptr) { ptr->OnFocusLost(); });
}

void Element::_OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyPress(key, scancode, mods);
	_VoidCallback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyPress(key, scancode, mods); });
}

void Element::_OnKeyRepeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	OnKeyRepeat(key, scancode, mods);
	_VoidCallback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyRepeat(key, scancode, mods); });
}

void Element::_OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {
	 OnKeyRelease(key, scancode, mods);
	 _VoidCallback([key, scancode, mods](const _CBPtr& ptr) { ptr->OnKeyRelease(key, scancode, mods); });
}

void Element::_OnCharacterInput(wchar_t character) {
	OnCharacterInput(character);
	_VoidCallback([character](const _CBPtr& ptr) { ptr->OnCharacterInput(character); });
}

bool Element::_OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {
	bool flag = OnMouseButtonPress(button, mods);
	flag |= _BoolCallback([button, mods](const _CBPtr& ptr) { return ptr->OnMouseButtonPress(button, mods); });
	return flag;
}

bool Element::_OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {
	bool flag = OnMouseButtonRelease(button, mods);
	flag |= _BoolCallback([button, mods](const _CBPtr& ptr) { return ptr->OnMouseButtonRelease(button, mods); });
	return flag;
}

void Element::_OnMouseEnter(const vec2& position) {
	OnMouseEnter(position);
	_VoidCallback([position](const _CBPtr& ptr) { ptr->OnMouseEnter(position); });
}

void Element::_OnMouseExit(const vec2& position) {
	OnMouseExit(position);
	_VoidCallback([position](const _CBPtr& ptr) { ptr->OnMouseExit(position); });
}

bool Element::_OnMouseMove(const vec2& position) {
	bool flag = OnMouseMove(position);
	flag |= _BoolCallback([position](const _CBPtr& ptr) { return ptr->OnMouseMove(position); });
	return flag;
}

bool Element::_OnMouseJump(const vec2& position) {
	bool flag = OnMouseJump(position);
	flag |= _BoolCallback([position](const _CBPtr& ptr) { return ptr->OnMouseJump(position); });
	return flag;
}

bool Element::_OnMouseDrag(const vec2& origin, const vec2& position) {
	bool flag = OnMouseDrag(origin, position);
	flag |= _BoolCallback([origin, position](const _CBPtr& ptr) { return ptr->OnMouseDrag(origin, position); });
	return flag;
}

bool Element::_OnMouseScroll(const vec2& scrollComponents) {
	bool flag = OnMouseScroll(scrollComponents);
	flag |= _BoolCallback([scrollComponents](const _CBPtr& ptr) { return ptr->OnMouseScroll(scrollComponents); });
	return flag;
}

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

void Element::_DrawTexturedTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const GLTexture2D& texture) {
	texture.Bind(0);
	_Draw({ v1, v2, v3 }, MODE_TEXTURED);
}

void Element::_DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const GLTexture2D& texture) {
	texture.Bind(0);
	_Draw({ v1, v2, v3, v3, v4, v1 }, MODE_TEXTURED);
}

void Element::_DrawTexturedQuad(const Bounds& bounds, const GLTexture2D& texture) {
	_DrawTexturedQuad(
			Vertex({ bounds.x, bounds.y }, { 0.0f, 1.0f }),
			Vertex({ bounds.x, bounds.y + bounds.height }, { 0.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y + bounds.height }, { 1.0f, 0.0f }),
			Vertex({ bounds.x + bounds.width, bounds.y }, { 1.0f, 1.0f }),
			texture);
}

void Element::_Draw(const std::vector<Vertex>& vertices, int mode) {
	_Initialize();

	_ui_shader->GetUniform("uiMode") = mode;
	_ui_vertex_data->SetData(vertices, GLBuffer::STREAM_DRAW);
	_ui_vao->Bind();

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Element::_Initialize() {
	if (_initialized) {
		return;
	}

	_ui_shader = std::make_unique<GLShaderProgram>();
	_ui_shader->AddShaderFromSource(GLShaderProgram::VERTEX, EngineResources::PreprocessShader("Shaders_uishader_vert_glsl"));
	_ui_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, EngineResources::PreprocessShader("Shaders_uishader_frag_glsl"));
	_ui_shader->Link();

	const DisplayConfiguration::Resolution& screenDimension = Engine::GetDisplayConfiguration().resolution;
	_ui_shader->GetUniform("screenDimensions") = vec2 { screenDimension.width, screenDimension.height };

	_ui_vertex_data = std::make_unique<GLBuffer>(GL::BufferTarget::ARRAY);
	_ui_vao = std::make_unique<GLVertexArray>();
	_ui_vao->SetVertexAttributes<vec2, vec4, vec2>(*_ui_vertex_data);

	_initialized = true;
}

}
