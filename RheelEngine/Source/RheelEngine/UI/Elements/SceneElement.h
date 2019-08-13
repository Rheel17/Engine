#ifndef SCENEELEMENT_H_
#define SCENEELEMENT_H_
#include "../../_common.h"

#include "../Element.h"
#include "../../Scene.h"
#include "../../Renderer/SceneRenderManager.h"

namespace rheel {

class RE_API SceneElement : public Element {

public:
	/**
	 * Creates a UI element that displays the camera output of the currently
	 * active scene. When the currently active scene switches, the camera
	 * switches to that scene automatically, looking for a camera with the given
	 * name.
	 */
	SceneElement(std::string cameraName);

	/**
	 * Creates a UI element that displays the camera output of the given scene.
	 * This does not automatically switch when the current active scene
	 * switches.
	 */
	SceneElement(Scene *scene, std::string cameraName);

	void Draw() const override;

	void OnFocusGained() override;

private:
	void _InitializeRenderer(const Bounds& bounds) const;

	bool _use_active_scene;
	mutable Scene *_scene;
	std::string _camera_name;

	mutable std::shared_ptr<SceneRenderer> _scene_renderer;

};

}

#endif
