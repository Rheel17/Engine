#ifndef SHADOWMAPDIRECTIONAL_H_
#define SHADOWMAPDIRECTIONAL_H_
#include "../_common.h"

#include "ShadowMap.h"

namespace rheel {

class RE_API ShadowMapDirectional : public ShadowMap {
	friend class ShadowMap;

public:
	~ShadowMapDirectional();

	void Update(const mat4& cameraMatrix, const mat4& cameraMatrixInv) override;

	const GLTexture2D& Texture() const;

private:
	ShadowMapDirectional(SceneRenderManager *manager, Light *light);

	mat4 _CalculateViewProjectionMatrix(const mat4& cameraMatrixInv) const;

	std::shared_ptr<GLFramebuffer> _shadow_buffer;

};

}

#endif
