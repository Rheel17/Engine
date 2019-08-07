#ifndef MODELRENDERCOMPONENT_H_
#define MODELRENDERCOMPONENT_H_
#include "../_common.h"

#include "RenderComponent.h"
#include "../Material.h"
#include "../Model.h"
#include "../Renderer/ModelRenderer.h"

namespace rheel {

/**
 * A component that renders a Model
 */
class RE_API ModelRenderComponent : public RenderComponent {

public:
	void SetModel(ModelPtr model);

	void SetMaterial(Material material);

	void OnAdd() override;

	void OnUpdateRenderers() override;

	void OnRemove() override;

private:
	ModelPtr _model;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;

};

}

#endif
