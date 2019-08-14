#ifndef SCRIPT_H_
#define SCRIPT_H_
#include "_common.h"

#include <memory>
#include <string>

namespace rheel {

class Scene;

class RE_API Script {
	friend class Scene;

public:
	virtual ~Script() = default;

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

	Scene& Parent() { return *_parent_scene; }

private:
	Scene *_parent_scene = nullptr;

public:
	static constexpr auto NAME_EULER_CAMERA_CONTROLLER = "engine:euler_camera_controller";

};

using ScriptPtr = std::shared_ptr<Script>;

}



#endif
