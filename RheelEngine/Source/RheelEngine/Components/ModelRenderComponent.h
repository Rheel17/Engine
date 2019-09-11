#ifndef MODELRENDERCOMPONENT_H_
#define MODELRENDERCOMPONENT_H_
#include "../_common.h"

#include "../Component.h"
#include "../Material.h"
#include "../ModelResource.h"
#include "../Renderer/ModelRenderer.h"

namespace rheel {

/**
 * A component that renders a Model
 */
class RE_API ModelRenderComponent : public Component {
	COMPONENT_INIT(ModelRenderComponent)

private:
	ModelRenderComponent(const ModelRenderComponent& modelRenderComponent);

public:
	ModelRenderComponent() = default;

	void SetModel(ModelResource& model);

	void SetMaterial(Material material);

	void OnAdd() override;

	void OnUpdateRenderers() override;

	void OnRemove() override;

private:
	ModelResource *_model = nullptr;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;

};

}

#endif
