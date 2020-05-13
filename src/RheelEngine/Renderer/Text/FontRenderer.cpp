/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "FontRenderer.h"

#include "Encoding.h"
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

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	resolve_vbo.SetData(triangles, sizeof(triangles));
	resolve_vao.SetVertexAttributes<vec2>(resolve_vbo);
}

FontRenderer::FontRenderer(Font& font) :
		_font(font),
		_glyph_buffer(font) {

	_transform_buffer.SetAllocationPolicy(gl::Buffer::AllocationPolicy::KEEP_BIGGER);
	_indirect_buffer.SetAllocationPolicy(gl::Buffer::AllocationPolicy::KEEP_BIGGER);

	_character_vao.SetVertexAttributes<vec3>(_glyph_buffer.GetGlyphBuffer());
	_character_vao.SetVertexAttributes<vec4>(_transform_buffer, 0, 4);
}

void FontRenderer::SetSize(unsigned int size) {
	_size = size;
}

void FontRenderer::SetColor(Color color) {
	_color = color;
}

int FontRenderer::Render(const char** text, int x, int y) {
	gl::ContextScope cs;

	// prepare the offscreen buffer
	auto screen = DisplayConfiguration::Get().resolution;
	ResizeBuffer_(screen.x, screen.y);

	// load the glyphs into the glyph buffer
	unsigned count = _glyph_buffer.Load(*text);
	if (count == 0) {
		return x;
	}

	// base transform
	float px = float(x) / screen.x * 2.0f - 1.0f;
	float py = float(y) / screen.y * -2.0f + 1.0f;
	float sx = float(_size) / screen.x * 2.0f;
	float sy = float(_size) / screen.y * 2.0f;

	// buffer space
	float bx = 400.0f / screen.x;
	float by = 400.0f / screen.y;

	// prepare the draw
	std::vector<unsigned> indices;
	std::vector<vec4> transforms;
	gl::DrawElementsIndirectBuffer::Commands commands;

	vec4 bounds(1.0f, 1.0f, -1.0f, -1.0f);

	for (unsigned i = 0; i < count; i++) {
		char32_t c = Encoding::Utf8ToCodePoint(*text);

		// add the indices to the index buffer
		// TODO: this can also be done by a glyph-buffer buffer
		unsigned indexOffset = indices.size();
		_glyph_buffer.AddIndices(c, indices);
		unsigned indexCount = indices.size() - indexOffset;

		// add the draw call
		commands.push_back({
				indexCount,  // count          (glyph size)
				4,           // instance_count (samples)
				indexOffset, // first_index    (glyph start)
				0,           // base_vertex
				i            // base_instance  (glyph index)
		});

		// transform the glyph
		transforms.push_back(vec4{ px, py, sx, sy });

		const auto& glyph = _glyph_buffer.GetLoadedGlyph(c);

		// increase the bounds of the resolve quad to include this glyph
		const vec4& glyphBounds = glyph.Bounds();
		bounds.x = std::min(bounds.x, glyphBounds.x * sx + px - bx);
		bounds.y = std::min(bounds.y, glyphBounds.y * sy + py - by);
		bounds.z = std::max(bounds.z, glyphBounds.z * sx + px + bx);
		bounds.w = std::max(bounds.w, glyphBounds.w * sy + py + by);

		// move to the next character
		x += glyph.Advance() * _size;
		px += glyph.Advance() * sx;
		*text += Encoding::Utf8Lenght(c);
	}

	// upload the buffers
	_character_vao.SetVertexIndices(indices);
	_transform_buffer.SetData(transforms);
	_indirect_buffer.SetData(commands);

	{
		// draw the glyphs
		gl::ContextScope cs1;

		gl::Context::Current().Enable(gl::Capability::COLOR_LOGIC_OP);
		gl::Context::Current().Disable(gl::Capability::BLEND);
		gl::Context::Current().SetLogicOp(gl::LogicOp::XOR);

		_static_data->text_buffer.Clear(gl::Framebuffer::BitField::COLOR);
		_static_data->draw_program.Use();
		_character_vao.DrawElementsIndirect(gl::VertexArray::Mode::TRIANGLES, _indirect_buffer, count);
	}

	// resolve to the main framebuffer
	_static_data->text_buffer.GetTextureAttachment(0).Bind(0);
	_static_data->resolve_program["bounds"] = bounds;
	_static_data->resolve_program["color"] = (vec4) _color;
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