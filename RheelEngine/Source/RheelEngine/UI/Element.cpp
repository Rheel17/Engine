#include "Element.h"

#include "../Engine.h"
#include "../Resources.h"

namespace rheel::ui {

#define MODE_COLORED    1
#define MODE_TEXTURED   2

std::shared_ptr<GLShaderProgram> Element::_ui_shader(nullptr);
std::shared_ptr<GLVertexArray> Element::_ui_vao(nullptr);
std::shared_ptr<GLBuffer> Element::_ui_vertex_data(nullptr);
bool Element::_initialized = false;

Element::Vertex::Vertex(vec2 position) :
		_position(std::move(position)), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(0.0f, 0.0f) {}

Element::Vertex::Vertex(vec2 position, vec4 color) :
		_position(std::move(position)), _color(std::move(color)), _texture(0.0f, 0.0f) {}

Element::Vertex::Vertex(vec2 position, vec2 texture) :
		_position(std::move(position)), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(std::move(texture)) {}


void Element::SetBounds(Bounds bounds){
	_has_initialized_bounds = true;
	_bounds = std::move(bounds);
}

const Element::Bounds& Element::GetBounds() const {
	return _bounds;
}

Element::Bounds& Element::GetBounds() {
	return _bounds;
}

void Element::InitializeBounds() {
	if (!_has_initialized_bounds) {
		std::tie(_bounds.width, _bounds.height) = GetDefaultDimensions();
		_bounds.x = 0;
		_bounds.y = 0;
		_has_initialized_bounds = true;
	}
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
	_ui_vertex_data->SetData(vertices, GLBuffer::DYNAMIC_DRAW);
	_ui_vao->Bind();

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Element::_Initialize() {
	if (_initialized) {
		return;
	}

	_ui_shader = std::make_shared<GLShaderProgram>();
	_ui_shader->AddShaderFromSource(GLShaderProgram::VERTEX, RESOURCE_AS_STRING(Shaders_uishader_vert_glsl));
	_ui_shader->AddShaderFromSource(GLShaderProgram::FRAGMENT, RESOURCE_AS_STRING(Shaders_uishader_frag_glsl));
	_ui_shader->Link();

	const DisplayConfiguration::Resolution& screenDimension = Engine::GetDisplayConfiguration().resolution;
	_ui_shader->GetUniform("screenDimensions") = vec2 { screenDimension.width, screenDimension.height };

	_ui_vertex_data = std::make_shared<GLBuffer>(GL::BufferTarget::ARRAY);
	_ui_vao = std::make_shared<GLVertexArray>();
	_ui_vao->SetVertexAttributes<vec2, vec4, vec2>(*_ui_vertex_data);

	_initialized = true;
}

}
