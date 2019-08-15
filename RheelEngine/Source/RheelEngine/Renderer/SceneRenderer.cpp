#include "SceneRenderer.h"

#include "SceneRenderManager.h"
#include "../Engine.h"
#include "../Resources.h"

namespace rheel {

SceneRenderer::SceneRenderer(SceneRenderManager *manager, std::string cameraName,
		unsigned width, unsigned height) :
		_manager(manager), _camera_name(std::move(cameraName)),
		_width(width), _height(height),
		_g_buffer(width, height, Engine::GetDisplayConfiguration().SampleCount(), true),
		_result_buffer(width, height) {

	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA, GL_FLOAT); // color
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB,  GL_FLOAT); // position
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB,  GL_FLOAT); // normal
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB,  GL_FLOAT); // material:ambient
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB,  GL_FLOAT); // material:diffuse
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB,  GL_FLOAT); // material:specular
	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA, GL_FLOAT); // material:parameters
	_g_buffer.AddRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_g_buffer.Create();

	_result_buffer.AddTexture(GL_RGB32F, GL_RGB, GL_FLOAT);
	_result_buffer.Create();
}

void SceneRenderer::SetSize(unsigned width, unsigned height) {
	if (_width == width && _height == height) {
		return;
	}

	_g_buffer = _g_buffer.ResizedCopy(width, height);
	_result_buffer = _result_buffer.ResizedCopy(width, height);
}

void SceneRenderer::Render(float dt) const {
	// get the camera
	CameraPtr camera = _manager->Scene()->GetCamera(_camera_name);

	// if no camera with the given name was found: don't render anything
	// new to the buffer.
	if (!camera) {
		return;
	}

	// send the camera matrix to the model shader
	GLShaderProgram& modelShader = ModelRenderer::GetModelShader();
	mat4 cameraMatrix = camera->CreateMatrix(_width, _height);
	modelShader["cameraMatrix"] = cameraMatrix;

	// write the scene to the g-buffer.
	_g_buffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& pair : _manager->RenderMap()) {
		pair.second.RenderObjects();
	}

	// write the g-buffer to the result buffer with the
	// lighting shader
	_result_buffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned index = 0;
	for (const auto& texture : _g_buffer.MultisampleTextures()) {
		texture.Bind(index++);
	}

	GLShaderProgram& shader = _manager->InitializedLightingShader();
	shader["gBufferTextureSize"] = ivec2 { _width, _height };
	shader["sampleCount"] = (GLint) Engine::GetDisplayConfiguration().SampleCount();
	shader["cameraPosition"] = camera->Position();

	_manager->DrawLightingQuad();

	GLFramebuffer::ClearBinding();
}

const GLTexture2D& SceneRenderer::OutputTexture() const {
	return _result_buffer.Textures()[0];
}

}
