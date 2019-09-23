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

	/**
	 * Sets the model to render.
	 */
	void SetModel(ModelResource& model);

	/**
	 * Sets the material to use when rendering.
	 */
	void SetMaterial(Material material);

	/**
	 * Returns the material currently in use.
	 */
	const Material& GetMaterial() const;

	/**
	 * Sets the scale of this model.
	 */
	void SetScale(const vec3& scale);

	/**
	 * Sets the scale of this model.
	 */
	void SetScale(float x, float y, float z);

	/**
	 * Sets the scale of this model.
	 */
	void SetScale(float scale);

	/**
	 * Returns the scale of this model.
	 */
	const vec3& Scale() const;

	void OnAdd() override;
	void OnUpdateRenderers() override;
	void OnRemove() override;

private:
	ModelResource *_model = nullptr;
	Material _material;
	ModelRenderer::ObjectDataPtr _object_data;
	vec3 _scale = vec3(1.0f, 1.0f, 1.0f);

};

}

#endif
