/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SHADOWMAPDIRECTIONAL_H
#define RHEELENGINE_SHADOWMAPDIRECTIONAL_H
#include "../_common.h"

#include "ShadowMap.h"

namespace rheel {

class RE_API ShadowMapDirectional : public ShadowMap {
	friend class SceneRenderManager;

public:
	~ShadowMapDirectional() override;

	void Update(Camera* camera, unsigned width, unsigned height) override;

	std::vector<std::reference_wrapper<const gl::Texture2D>> Textures() const;

	const std::vector<mat4>& LightMatrices() const;

	float Bias() const;

private:
	ShadowMapDirectional(SceneRenderManager* manager, Light* light);

	void CalculateViewProjectionMatrices_(Camera* camera, unsigned width, unsigned height);

	std::vector<unsigned> _csm_split;
	std::vector<float> _csm_borders;
	unsigned _csm_count;
	float _bias;

	std::vector<gl::Framebuffer> _shadow_buffers;
	std::vector<mat4> _light_matrices;

};

}

#endif
