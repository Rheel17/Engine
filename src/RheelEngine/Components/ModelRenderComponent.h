/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_MODELRENDERCOMPONENT_H
#define RHEELENGINE_MODELRENDERCOMPONENT_H
#include "../_common.h"

#include "../Component.h"
#include "../Material.h"
#include "../Assets/Model.h"
#include "../Renderer/ModelRenderer.h"

namespace rheel {

/**
 * A component that renders a Model
 */
class RE_API ModelRenderComponent : public Component {

public:
	ModelRenderComponent(Model model, Material material);

	/**
	 * Sets the material to use when rendering.
	 */
	void SetMaterial(Material material);

	/**
	 * Returns the material currently in use.
	 */
	const Material& GetMaterial() const;

	void Activate() override;
	void Render() override;
	void Deactivate() override;

private:
	Model _model;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;

};

}

#endif
