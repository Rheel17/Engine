#ifndef MODELRENDERCOMPONENT_H_
#define MODELRENDERCOMPONENT_H_
#include "../_common.h"
#include "../Components/RenderComponent.h"
#include "../Material.h"
#include "../Model.h"
#include "../Renderer/ModelRenderer.h"

namespace rheel {

/**
 * A component that renders a Model
 */
class RE_API ModelRenderComponent : public RenderComponent {
	COMPONENT_INIT(ModelRenderComponent)

private:
	ModelRenderComponent(const ModelRenderComponent& modelRenderComponent);

public:
	ModelRenderComponent() = default;

	void SetModel(std::shared_ptr<Model> model);

	void SetMaterial(Material material);

	void OnAdd() override;

	void OnUpdateRenderers() override;

	void OnRemove() override;

private:
	std::shared_ptr<Model> _model;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;

};

}

#endif
