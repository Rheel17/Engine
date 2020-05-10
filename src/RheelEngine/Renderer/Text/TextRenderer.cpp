/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "TextRenderer.h"

#include "Encoding.h"
#include "../Display/DisplayConfiguration.h"
#include "../OpenGL/Context.h"
#include "../../EngineResources.h"

namespace rheel {

TextRenderer::ogl_data::ogl_data() :
		resolve_vbo(gl::Buffer::Target::ARRAY),
		text_buffer(1, 1) {

	gl::ContextScope cs;

	shader.AttachShader(gl::Shader::ShaderType::FRAGMENT, EngineResources::PreprocessShader("Shaders_fontshader_frag_glsl"));
	shader.AttachShader(gl::Shader::ShaderType::VERTEX, EngineResources::PreprocessShader("Shaders_fontshader_vert_glsl"));
	shader.Link();
	shader["textBuffer"] = 0;

	GLfloat triangles[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
	resolve_vbo.SetData(triangles, sizeof(triangles));
	resolve_vao.SetVertexAttributes<vec2>(resolve_vbo);

	text_buffer.AttachTexture(gl::InternalFormat::RGBA, gl::Format::RGBA, 0);
	text_buffer.AttachRenderbuffer(gl::InternalFormat::DEPTH24_STENCIL8, gl::Framebuffer::Attachment::DEPTH_STENCIL);
	text_buffer.SetDrawBuffers({ 0 });
}

// This text rendering system is based on the amazing blog post by Evan Wallace:
// https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac

void TextRenderer::DrawText(Font& font, const Color& color, const std::string& text, int x, int y, unsigned size) {
	DrawText(font, color, text.c_str(), x, y, size);
}

void TextRenderer::DrawText(Font& font, const Color& color, const char* text, int x, int y, unsigned int size) {
	while (*text != 0) {
		x = DrawChars_(font, color, &text, x, y, size);
	}
}

void TextRenderer::ResizeBuffer_(unsigned width, unsigned height) const {
	if (_ogl_data->text_buffer.GetViewportWidth() != width || _ogl_data->text_buffer.GetViewportHeight() != height) {
		_ogl_data->text_buffer = gl::Framebuffer(_ogl_data->text_buffer, width, height);
	}
}

int TextRenderer::DrawChars_(Font& font, const Color& color, const char** text, int x, int y, unsigned size) {
	auto screen = DisplayConfiguration::Get().resolution;
	ResizeBuffer_(screen.x, screen.y);

	GlyphBuffer& buffer = *_glyph_buffers.Get(&font, [](Font* font){
		return std::make_unique<GlyphBuffer>(*font);
	});

	size_t count = buffer.Load(*text);

	for (size_t i = 0; i < count; i++) {
		char32_t c = Encoding::Utf8ToCodePoint(*text);
		*text += Encoding::Utf8Lenght(c);
	}

	return x;
}

}
