/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "FontRenderer.h"

#include "../Display/DisplayConfiguration.h"
#include "../OpenGL/Context.h"
#include "../../EngineResources.h"

namespace rheel {

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

Cache<const Font*, FontRenderer> FontRenderer::_renderers;

FontRenderer::static_data::static_data() :
		text_buffer(1, 1),
		resolve_vbo(gl::Buffer::Target::ARRAY) {

	gl::ContextScope cs;

	text_buffer.AttachTexture(gl::InternalFormat::R8, gl::Format::RED, 0);
	text_buffer.SetDrawBuffers({ 0 });

	draw_program.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_draw_vert_glsl"));
	draw_program.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_draw_frag_glsl"));
	draw_program.Link();

	resolve_program.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_resolve_vert_glsl"));
	resolve_program.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_resolve_frag_glsl"));
	resolve_program.Link();
	resolve_program["color"] = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	resolve_vbo.SetData(triangles, sizeof(triangles));
	resolve_vao.SetVertexAttributes<vec2>(resolve_vbo);
}

FontRenderer::FontRenderer(const Font& font) :
		_font(font) {

	_glyph_buffer.SetData(font._glyph_vertices);
	_character_vao.SetVertexAttributes<vec3>(_glyph_buffer);
	_character_vao.SetVertexAttributes<vec2>(_transform_buffer, 0, sample_count);
	_character_vao.SetVertexIndices(font._glyph_indices);
}

void FontRenderer::SetSize(unsigned int size) {
	_size = size;
}

void FontRenderer::SetColor(Color color) {
	if (_color != color) {
		gl::ContextScope cs;
		_color = color;
		_static_data->resolve_program["color"] = (vec4) color;
	}
}

const gl::VertexArray& FontRenderer::GetCharacterVao() const {
	return _character_vao;
}

const gl::Buffer& FontRenderer::GetGlyphBuffer() const {
	return _glyph_buffer;
}

void FontRenderer::Render(std::u32string_view text, int x, int y, unsigned width, TextAlign align) {
	auto screen = DisplayConfiguration::Get().resolution;
	vec4 bounds;

	float sx = static_cast<float>(2 * _size) / static_cast<float>(screen.x);
	float sy = static_cast<float>(2 * _size) / static_cast<float>(screen.y);
	float tx = (static_cast<float>(x) / static_cast<float>(screen.x)) * 2.0f - 1.0f;
	float ty = (static_cast<float>(y) / static_cast<float>(screen.y)) * -2.0f + 1.0f;

	float fwidth = width == std::numeric_limits<unsigned>::max()
			? std::numeric_limits<float>::max()
			: static_cast<float>(width) / static_cast<float>(_size);

	unsigned count = PreparedText::Prepare(PreparedText::prepare_text_input{
		.text = text,
		.width = fwidth,
		.align = align,
		.font = std::ref(_font)
	}, _transform_buffer, _indirect_buffer, bounds);

	mat3 transform(
			sx,   0.0f, 0.0f,
			0.0f, sy,   0.0f,
			tx,   ty,   1.0f
	);

	_render(_character_vao, _indirect_buffer, bounds, count, transform);
}

void FontRenderer::_render(const gl::VertexArray& vao, const gl::DrawElementsIndirectBuffer& indirect_buffer,
		const vec4& bounds, unsigned count, const mat3& transform) {

	auto screen = DisplayConfiguration::Get().resolution;
	_resize_buffer(screen.x, screen.y);

	{
		gl::ContextScope cs_1;
		_static_data->text_buffer.BindForDrawing();

		// clear the text buffer
		{
			vec3 transformed_bounds_xy = transform * vec3(bounds.x, bounds.y, 1.0f);
			vec3 transformed_bounds_zw = transform * vec3(bounds.z, bounds.w, 1.0f);

			gl::ContextScope cs_2;
			gl::Context::Current().Enable(gl::Capability::SCISSOR_TEST);
			gl::Context::Current().SetScissorTest(
					(0.5f + 0.5f * transformed_bounds_xy.x) * screen.x - 4,
					(0.5f + 0.5f * transformed_bounds_xy.y) * screen.y - 4,
					((transformed_bounds_zw.x - transformed_bounds_xy.x) * 0.5f) * screen.x + 8,
					((transformed_bounds_zw.y - transformed_bounds_xy.y) * 0.5f) * screen.y + 8
			);

			_static_data->text_buffer.Clear(gl::Framebuffer::BitField::COLOR);
		}

		gl::Context::Current().Disable(gl::Capability::BLEND);
		gl::Context::Current().Enable(gl::Capability::COLOR_LOGIC_OP);
		gl::Context::Current().SetLogicOp(gl::LogicOp::XOR);

		// draw the characters
		_static_data->draw_program["transform"] = transform;
		_character_vao.DrawElementsIndirect(gl::VertexArray::Mode::TRIANGLES, _indirect_buffer, count);
	}

	// resolve to the main framebuffer
	_static_data->text_buffer.GetTextureAttachment(0).Bind(0);
	_static_data->resolve_program["bounds"] = bounds;
	_static_data->resolve_program["transform"] = transform;
	_static_data->resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);
}

void FontRenderer::_resize_buffer(unsigned width, unsigned height) {
	if (_static_data->text_buffer.GetViewportWidth() != width || _static_data->text_buffer.GetViewportHeight() != height) {
		_static_data->text_buffer = gl::Framebuffer(_static_data->text_buffer, width, height);
		_static_data->draw_program["bufferSize"] = vec2(width, height);
	}
}

FontRenderer& FontRenderer::_get(const Font& font) {
	return _renderers.Get(&font, [](const Font* font) {
		return FontRenderer(*font);
	});
}

}