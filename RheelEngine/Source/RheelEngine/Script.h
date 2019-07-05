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

	// TODO: add script parameters

	/**
	 * TODO: Called when the scene is updated, before the scene objects are
	 * updated.
	 */
	virtual void PreOnUpdate() {}

	/**
	 * TODO: Called when the scene is updated, after the scene objects are
	 * updated.
	 */
	virtual void PostOnUpdate() {}

protected:
	Script() = default;

	Scene& Parent() { return *_parent_scene; }

private:
	Scene *_parent_scene = nullptr;

};

using ScriptPtr = std::shared_ptr<Script>;

}



#endif
