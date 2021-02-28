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
	// gen_component_id
	static constexpr const ComponentId id = 65284;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

	ModelRenderComponent(Model model, Material material);
	~ModelRenderComponent() = default;

	RE_NO_COPY(ModelRenderComponent);
	RE_DEFAULT_MOVE(ModelRenderComponent);

	/**
	 * Sets the material to use when rendering.
	 */
	void SetMaterial(Material material);

	/**
	 * Returns the material currently in use.
	 */
	const Material& GetMaterial() const;

protected:
	void OnActivate() override;
	void OnDeactivate() override;
	void Update() override;

private:
	Model _model;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;

};

}

#endif
