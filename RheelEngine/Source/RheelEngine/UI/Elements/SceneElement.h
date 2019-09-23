#ifndef SCENEELEMENT_H_
#define SCENEELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class SceneRenderer;
class Scene;

class RE_API SceneElement : public Element {
	RE_NO_COPY(SceneElement);

public:
	SceneElement(SceneElement&&) = default;

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

	/**
	 * Sets whether this element should grab the mouse on focus. Its default
	 * value is true.
	 */
	void SetGrabOnFocus(bool grabOnFocus);

	/**
	 * Returns whether this element should grab the mouse on focus.
	 */
	bool GetGrabOnFocus() const;

	void Draw(float dt) const override;

	// Override methods to pass input events to scripts
	void OnFocusGained() override;
	void OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) override;
	void OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) override;
	bool OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) override;
	bool OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) override;
	bool OnMouseMove(const vec2& position) override;
	bool OnMouseScroll(const vec2& scrollComponents) override;

private:
	void _InitializeRenderer(const Bounds& bounds) const;

	bool _use_active_scene;
	mutable Scene *_scene;
	std::string _camera_name;

	bool _grab_on_focus = true;

	mutable std::shared_ptr<SceneRenderer> _scene_renderer;

};

}

#endif
