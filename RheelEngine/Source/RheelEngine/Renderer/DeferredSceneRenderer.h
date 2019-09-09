#ifndef DEFERREDSCENERENDERER_H_
#define DEFERREDSCENERENDERER_H_
#include "../_common.h"

#include "SceneRenderer.h"

namespace rheel {

class RE_API DeferredSceneRenderer : public SceneRenderer {
	friend class SceneRenderManager;

public:
	void Render(float dt) override;

protected:
	GLFramebuffer CreateResultBuffer(unsigned width, unsigned height) const;

	void Resize(unsigned width, unsigned height) override;

private:
	DeferredSceneRenderer(SceneRenderManager *manager, std::string cameraName, unsigned width, unsigned height);

	GLFramebuffer _g_buffer;

};

}

#endif
