#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include "ObjectPtr.h"

#include <memory>
#include <string>

namespace rheel {

class RE_API Component {
	friend class Object;

public:
	virtual ~Component() = default;

	/**
	 * Called when the object is added to a scene
	 */
	virtual void OnAdd() {}

	/**
	 * Called when the scene is updated, and gives a chance to this component to
	 * update its state and/or its object. Updates are received in the same
	 * order as the components were added to the object or its blueprint.
	 */
	virtual void OnUpdate() {}

	/**
	 * Called at most once per frame to update renderers.
	 */
	virtual void OnUpdateRenderers() {}

	/**
	 * TODO: Called every time the object is rendered.
	 */
	virtual void OnRender() {}

	/**
	 * Called when the object is removed from the scene
	 */
	virtual void OnRemove() {}

protected:
	Component() = default;

	ObjectPtr Parent() { return _parent_object; }

private:
	ObjectPtr _parent_object;

public:
	static constexpr auto NAME_MODELRENDER = "engine:meshrender";
	static constexpr auto NAME_RIGIDBODY = "engine:rigidbody";

};

using ComponentPtr = std::shared_ptr<Component>;

}

#endif
