#ifndef SCENERENDERER_H_
#define SCENERENDERER_H_
#include "../_common.h"

#include "../Scene.h"
#include "OpenGL/GLFramebuffer.h"
#include "OpenGL/GLShaderProgram.h"
#include "OpenGL/GLVertexArray.h"
#include "OpenGL/GLBuffer.h"

namespace rheel {

class SceneRenderManager;

class RE_API SceneRenderer {
	friend class SceneRenderManager;

public:
	void SetSize(unsigned width, unsigned height);

	void Render() const;

	const GLTexture2D& OutputTexture() const;

private:
	SceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height);

	SceneRenderManager *_manager;
	std::string _camera_name;
	unsigned _width;
	unsigned _height;

	GLFramebuffer _g_buffer;
	GLFramebuffer _result_buffer;

};

}

#endif
