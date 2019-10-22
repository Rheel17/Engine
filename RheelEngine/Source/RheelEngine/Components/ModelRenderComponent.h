///*
// * Copyright © 2019 Levi van Rheenen. All rights reserved.
// */
//#ifndef MODELRENDERCOMPONENT_H_
//#define MODELRENDERCOMPONENT_H_
//#include "../_common.h"
//
//#include "../Component.h"
//#include "../Material.h"
//#include "../ModelResource.h"
//#include "../Renderer/ModelRenderer.h"
//
//namespace rheel {
//
///**
// * A component that renders a Model
// */
//class RE_API ModelRenderComponent : public Component {
//
//public:
//	/**
//	 * Sets the model to render.
//	 */
//	void SetModel(ModelResource& model);
//
//	/**
//	 * Sets the material to use when rendering.
//	 */
//	void SetMaterial(Material material);
//
//	/**
//	 * Returns the material currently in use.
//	 */
//	const Material& GetMaterial() const;
//
//	void OnAdd() override;
//	void OnRender() override;
//	void OnRemove() override;
//
//private:
//	ModelResource *_model = nullptr;
//	Material _material;
////	ModelRenderer::ObjectDataPtr _object_data;
//
//};
//
//}
//
//#endif
