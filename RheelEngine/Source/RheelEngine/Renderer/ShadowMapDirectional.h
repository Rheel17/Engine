#ifndef SHADOWMAPDIRECTIONAL_H_
#define SHADOWMAPDIRECTIONAL_H_
#include "../_common.h"

#include "ShadowMap.h"

namespace rheel {

class RE_API ShadowMapDirectional : public ShadowMap {
	friend class SceneRenderManager;

public:
	~ShadowMapDirectional();

	void Update(CameraPtr camera, unsigned width, unsigned height) override;

	std::vector<GLTexture2D> Textures() const;

	const std::vector<mat4>& LightMatrices() const;

	float Bias() const;

private:
	ShadowMapDirectional(SceneRenderManager *manager, LightPtr light);

	void _CalculateViewProjectionMatrices(CameraPtr camera, unsigned width, unsigned height);

	std::vector<unsigned> _csm_split;
	std::vector<float> _csm_borders;
	unsigned _csm_count;
	float _bias;

	std::vector<GLFramebuffer> _shadow_buffers;
	std::vector<mat4> _light_matrices;

public:
	static const GLTexture2D& EmptyShadowMap();

private:
	static std::shared_ptr<GLTexture2D> _empty_shadow_map;

};

}

#endif
