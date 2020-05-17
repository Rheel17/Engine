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

FontRenderer::static_data::static_data() :
		text_buffer(1920, 1080),
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

FontRenderer::FontRenderer(Font& font) :
		_font(font) {

	_glyph_buffer.SetData(font._glyph_vertices);
	_character_vao.SetVertexAttributes<vec3>(_glyph_buffer);
	_character_vao.SetVertexAttributes<vec4>(_transform_buffer, 0, 4);
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

int FontRenderer::Render(const char32_t** text, size_t count, int x, int y) {
	if (count == 0 || count > MAX_CHARS) {
		count = MAX_CHARS;
	}

	gl::ContextScope cs;

	// prepare the offscreen buffer
	auto screen = DisplayConfiguration::Get().resolution;
	ResizeBuffer_(screen.x, screen.y);

	// base transform
	float px = float(x) / screen.x * 2.0f - 1.0f;
	float py = float(y) / screen.y * -2.0f + 1.0f;
	float sx = float(_size) / screen.x * 2.0f;
	float sy = float(_size) / screen.y * 2.0f;

	// buffer space
	float bx = 4.0f / screen.x;
	float by = 4.0f / screen.y;

	// prepare the draw
	vec4 bounds(1.0f, 1.0f, -1.0f, -1.0f);

	size_t numChars = 0;
	for (unsigned i = 0; **text && i < count; i++) {
		const auto&[offset, glyphSize] = _font._glyph_offsets[_font._glyph_index[**text]];

		// add the draw call
		numChars++;
		_commands[i] = {
				glyphSize,  // count          (glyph size)
				4,          // instance_count (samples)
				offset,     // first_index    (glyph start)
				0,          // base_vertex
				i           // base_instance  (glyph index)
		};

		// transform the glyph
		_transforms[i] = { px, py, sx, sy };

		const auto& glyph = _font._glyphs[_font._glyph_index[**text]];

		// increase the bounds of the resolve quad to include this glyph
		const vec4& glyphBounds = glyph.Bounds();
		bounds.x = std::min(bounds.x, glyphBounds.x * sx + px - bx);
		bounds.y = std::min(bounds.y, glyphBounds.y * sy + py - by);
		bounds.z = std::max(bounds.z, glyphBounds.z * sx + px + bx);
		bounds.w = std::max(bounds.w, glyphBounds.w * sy + py + by);

		// move to the next character
		x += glyph.Advance() * _size;
		px = float(x) / screen.x * 2.0f - 1.0f;
		(*text)++;
	}

	// upload the buffers
	_transform_buffer.SetData(_transforms.data(), numChars, gl::Buffer::Usage::STREAM_DRAW);
	_indirect_buffer.SetData(_commands.data(), numChars, gl::Buffer::Usage::STREAM_DRAW);

	{
		gl::ContextScope cs1;
		_static_data->text_buffer.BindForDrawing();

		// clear the text buffer
		{
			gl::ContextScope cs2;
			gl::Context::Current().Enable(gl::Capability::SCISSOR_TEST);
			gl::Context::Current().SetScissorTest(
					(0.5f + 0.5f * bounds.x) * screen.x - 4,
					(0.5f + 0.5f * bounds.y) * screen.y - 4,
					((bounds.z - bounds.x) * 0.5f) * screen.x + 8,
					((bounds.w - bounds.y) * 0.5f) * screen.y + 8
			);

			_static_data->text_buffer.Clear(gl::Framebuffer::BitField::COLOR);
		}

		gl::Context::Current().Disable(gl::Capability::BLEND);
		gl::Context::Current().Enable(gl::Capability::COLOR_LOGIC_OP);
		gl::Context::Current().SetLogicOp(gl::LogicOp::XOR);

		// draw the characters
		_static_data->draw_program.Use();
		_character_vao.DrawElementsIndirect(gl::VertexArray::Mode::TRIANGLES, _indirect_buffer, numChars);
	}

	// resolve to the main framebuffer
	_static_data->text_buffer.GetTextureAttachment(0).Bind(0);
	_static_data->resolve_program["bounds"] = bounds;
	_static_data->resolve_vao.DrawArrays(gl::VertexArray::Mode::TRIANGLES, 0, 6);

	return x;
}

void FontRenderer::ResizeBuffer_(unsigned width, unsigned height) {
	if (_static_data->text_buffer.GetViewportWidth() != width || _static_data->text_buffer.GetViewportHeight() != height) {
		_static_data->text_buffer = gl::Framebuffer(_static_data->text_buffer, width, height);
		_static_data->draw_program["bufferSize"] = vec2(width, height);
	}
}

}