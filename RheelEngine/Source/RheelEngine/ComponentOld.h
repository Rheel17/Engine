/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef COMPONENTOLD_H_
#define COMPONENTOLD_H_
#include "_common.h"

#define COMPONENT_INIT(Class)						\
protected:											\
	ComponentOld *__CloneHeap() const override {	\
		return new Class(*this); 					\
	}												\
private:

namespace rheel {

class Object;

class RE_API ComponentOld {
	friend class Blueprint;
	friend class Object;

public:
	virtual ~ComponentOld() = default;

	Object *Parent() const;

	/**
	 * Called when the object is added to a scene
	 */
	virtual void OnAdd() {}

	/**
	 * Called when the scene is updated, and gives a chance to this component to
	 * update its state and/or its object. Updates are received in the same
	 * order as the components were added to the object or its blueprint.
	 */
	virtual void OnUpdate(float dt) {}

	/**
	 * Called at most once per frame to update renderers.
	 */
	virtual void OnUpdateRenderers() {}

	/**
	 * Called when the object is removed from the scene
	 */
	virtual void OnRemove() {}

protected:
	ComponentOld() = default;

	virtual ComponentOld *__CloneHeap() const = 0;

private:
	Object *_parent_object = nullptr;

};

}

#endif
