#include "SceneRenderer.h"

#include <set>

#include "ShadowMapDirectional.h"
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

	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA); // color
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // position
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // normal
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // material:ambient
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // material:diffuse
	_g_buffer.AddTexture(GL_RGB32F,  GL_RGB ); // material:specular
	_g_buffer.AddTexture(GL_RGBA32F, GL_RGBA); // material:parameters
	_g_buffer.AddRenderbuffer(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	_g_buffer.Create();

	_result_buffer.AddTexture(GL_RGB32F, GL_RGB);
	_result_buffer.Create();
}

void SceneRenderer::SetSize(unsigned width, unsigned height) {
	if (_width == width && _height == height) {
		return;
	}

	_g_buffer = _g_buffer.ResizedCopy(width, height);
	_result_buffer = _result_buffer.ResizedCopy(width, height);
}

void SceneRenderer::Render(float dt) {
	// get the camera
	CameraPtr camera = _manager->Scene()->GetCamera(_camera_name);

	// if no camera with the given name was found: don't render anything
	// new to the buffer.
	if (!camera) {
		return;
	}

	// update the shadow maps
	mat4 matrix = camera->CreateMatrix(_width, _height);

	if (_manager->ShouldDrawShadows()) {
		_CorrectShadowMaps();

		for (auto iter : _shadow_maps) {
			iter.second->Update(camera, _width, _height);
		}
	}

	// send the camera matrix to the model shader
	mat4 cameraMatrix = matrix;//camera->CreateMatrix(_width, _height);
	GLShaderProgram& modelShader = ModelRenderer::GetModelShader();
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

	// bind the shadow objects
	auto sm = std::dynamic_pointer_cast<ShadowMapDirectional>(_shadow_maps.begin()->second);
	sm->Texture().Bind(index++);
	shader["lightspaceMatrix"] = sm->LightMatrix();

	_manager->DrawLightingQuad();

	GLShaderProgram::ClearUse();
	GLFramebuffer::ClearBinding();
}

const GLTexture2D& SceneRenderer::OutputTexture() const {
	return _result_buffer.Textures()[0];
}

void SceneRenderer::_CorrectShadowMaps() {
	std::set<std::string> lightNames;
	for (auto pair : _shadow_maps) {
		lightNames.insert(pair.first);
	}

	for (const std::string& lightName : _manager->Scene()->Lights()) {
		LightPtr light = _manager->Scene()->GetLight(lightName);

		if (!light->CastsShadows()) {
			continue;
		}

		lightNames.erase(lightName);

		if (_shadow_maps.find(lightName) == _shadow_maps.end()) {
			_shadow_maps.insert({ lightName, _manager->CreateShadowMap(lightName) });
		}
	}

	for (const std::string& lightName : lightNames) {
		_shadow_maps.erase(lightName);
	}
}

}
