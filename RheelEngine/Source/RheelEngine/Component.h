#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "_common.h"

#include <string>

#define COMPONENT_INIT(Class)						\
protected:											\
	Component *__CloneHeap() const override {		\
		return new Class(*this); 					\
	}												\
private:

namespace rheel {

class Object;

class RE_API Component {
	friend class Blueprint;
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
	 * Called when the object is removed from the scene
	 */
	virtual void OnRemove() {}

protected:
	Component() = default;

	virtual Component *__CloneHeap() const = 0;

	Object& Parent() { return *_parent_object; }

private:
	Object *_parent_object = nullptr;

};

}

#endif
