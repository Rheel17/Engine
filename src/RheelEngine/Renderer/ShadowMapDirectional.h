/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef SHADOWMAPDIRECTIONAL_H_
#define SHADOWMAPDIRECTIONAL_H_
#include "../_common.h"

#include "ShadowMap.h"

namespace rheel {

class RE_API ShadowMapDirectional : public ShadowMap {
	friend class SceneRenderManager;

public:
	~ShadowMapDirectional();

	void Update(Camera *camera, unsigned width, unsigned height) override;

	std::vector<_GLTexture2D> Textures() const;

	const std::vector<mat4>& LightMatrices() const;

	float Bias() const;

private:
	ShadowMapDirectional(SceneRenderManager *manager, Light *light);

	void _CalculateViewProjectionMatrices(Camera *camera, unsigned width, unsigned height);

	std::vector<unsigned> _csm_split;
	std::vector<float> _csm_borders;
	unsigned _csm_count;
	float _bias;

	std::vector<_GLFramebuffer> _shadow_buffers;
	std::vector<mat4> _light_matrices;

public:
	static const _GLTexture2D& EmptyShadowMap();

private:
	static std::unique_ptr<_GLTexture2D> _empty_shadow_map;

};

}

#endif
