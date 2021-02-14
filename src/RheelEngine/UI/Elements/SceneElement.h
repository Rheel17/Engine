/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SCENEELEMENT_H
#define RHEELENGINE_SCENEELEMENT_H
#include "../../_common.h"
#include "../../Renderer/PostProcessingStack.h"
#include "../../Scene.h"

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
	explicit SceneElement(ConstEntityId camera_entity);

	/**
	 * Creates a UI element that displays the camera output of the given scene.
	 * This does not automatically switch when the current active scene
	 * switches.
	 */
	SceneElement(Scene* scene, ConstEntityId camera_entity);

	/**
	 * Sets whether this element should grab the mouse on focus. Its default
	 * value is true.
	 */
	void SetGrabOnFocus(bool grabOnFocus);

	/**
	 * Returns whether this element should grab the mouse on focus.
	 */
	bool GetGrabOnFocus() const;

	/**
	 * Returns the post processing stack for this scene element. The reference
	 * result can be changed to change the post processing stack.
	 */
	PostProcessingStack& GetPostProcessingStack();

	void DoDraw(float time, float dt) const override;

	// Override methods to pass input events to scripts
	void OnFocusGained() override;
	void OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) override;
	void OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) override;
	void OnCharacterInput(Input::Unicode character) override;
	void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) override;
	void OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) override;
	void OnMouseMove(const vec2& position) override;
	void OnMouseJump(const vec2& position) override;
	void OnMouseDrag(const vec2& origin, const vec2& position) override;
	void OnMouseScroll(const vec2& scrollComponents) override;

private:
	void InitializeRenderer_(const Bounds& bounds) const;

	bool _use_active_scene;
	mutable Scene* _scene;
	ConstEntityId _camera_entity;

	bool _grab_on_focus = true;

	mutable std::shared_ptr<PostProcessingStack> _post_processing_stack;
	mutable std::shared_ptr<SceneRenderer> _scene_renderer;

};

}

#endif
