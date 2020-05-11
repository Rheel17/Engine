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
		text_buffer(1920, 1080) {

	gl::ContextScope cs;

	text_buffer.AttachTexture(gl::InternalFormat::RGBA, gl::Format::RGBA, 0);
	text_buffer.SetDrawBuffers({ 0 });

	draw_program.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_draw_vert_glsl"));
	draw_program.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_draw_frag_glsl"));
	draw_program.Link();

	resolve_program.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_resolve_vert_glsl"));
	resolve_program.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_resolve_frag_glsl"));
	resolve_program.Link();
}

FontRenderer::FontRenderer(Font& font) :
		_font(font),
		_glyph_buffer(font) {

	_transform_buffer.SetAllocationPolicy(gl::Buffer::AllocationPolicy::KEEP_BIGGER);
	_indirect_buffer.SetAllocationPolicy(gl::Buffer::AllocationPolicy::KEEP_BIGGER);

	_character_vao.SetVertexAttributes<vec3>(_glyph_buffer.GetGlyphBuffer());
	_character_vao.SetVertexAttributes<vec4>(_transform_buffer, 0, true);
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
	size_t count = _glyph_buffer.Load(*text);

	// draw
	for (size_t i = 0; i < count; i++) {
		char32_t c = Encoding::Utf8ToCodePoint(*text);



		*text += Encoding::Utf8Lenght(c);
	}

	return x;
}

void FontRenderer::ResizeBuffer_(unsigned width, unsigned height) {
	if (_static_data->text_buffer.GetViewportWidth() != width || _static_data->text_buffer.GetViewportHeight() != height) {
		_static_data->text_buffer = gl::Framebuffer(_static_data->text_buffer, width, height);
	}
}

}