#ifndef MODELRENDERCOMPONENT_H_
#define MODELRENDERCOMPONENT_H_
#include "../_common.h"

#include "RenderComponent.h"
#include "../Model.h"
#include "../Renderer/ModelRenderer.h"

namespace rheel {

/**
 * A component that renders a Model
 */
class RE_API ModelRenderComponent : public RenderComponent {

public:
	void SetModel(ModelPtr model);

	void OnAdd() override;

	void OnUpdateRenderers() override;

	void OnRemove() override;

private:
	ModelPtr _model;
	ModelRenderer::ObjectData *_object_data;

};

}

#endif
