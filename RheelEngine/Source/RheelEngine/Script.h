#ifndef SCRIPT_H_
#define SCRIPT_H_
#include "_common.h"

#define SCRIPT_INIT(Class)						\
protected:										\
	Script *__CloneHeap() const override {		\
		return new Class(*this); 				\
	}											\
private:

namespace rheel {

class Scene;

class RE_API Script {
	friend class Scene;

public:
	virtual ~Script() = default;

	/**
	 * Called when this script is first put in use, either when the scene it
	 * belongs to is initialized, or when it is added to an active scene.
	 */
	virtual void Initialize() {}

	/**
	 * Called when the scene is updated, before the scene objects are updated.
	 */
	virtual void PreOnUpdate() {}

	/**
	 * Called when the scene is updated, after the scene objects are updated.
	 */
	virtual void PostOnUpdate() {}

protected:
	Script() = default;

	virtual Script *__CloneHeap() const = 0;

	Scene& Parent() { return *_parent_scene; }
	float TimeDelta() { return _dt; }
	float Time() { return _time; }

private:
	Scene *_parent_scene = nullptr;
	float _dt = 0.016f;
	float _time = 0.0f;

};

}



#endif
