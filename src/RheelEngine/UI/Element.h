/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ELEMENT_H
#define RHEELENGINE_ELEMENT_H
#include "../_common.h"

#include <type_traits>

#include "InputCallback.h"
#include "../Color.h"
#include "../Animator/Animator.h"
#include "../Assets/Image.h"
#include "../Assets/Shader.h"
#include "../Renderer/OpenGL/Program.h"
#include "../Renderer/OpenGL/VertexArray.h"
#include "../Renderer/OpenGL/Texture2D.h"
#include "../Renderer/Text/TextRenderer.h"
#include "../Util/Cache.h"

namespace rheel {

class Container;
class UI;

class RE_API Element : public InputCallback {
	RE_NO_COPY(Element);

	friend class UI;
	friend class Container;

	struct RE_API ogl_data {
		ogl_data();
		gl::Program ui_shader;
		gl::VertexArray ui_vao;
		gl::Buffer ui_vertex_data;
		Cache<std::uintptr_t, gl::Program> custom_shaders;
		TextRenderer text_renderer;
	};
	mutable pseudo_static_pointer<ogl_data> _ogl_data;

private:
	using CbPtr = std::shared_ptr<InputCallback>;

public:
	class Bounds {

	public:
		unsigned x, y;
		unsigned width, height;

		bool operator==(const Bounds& bounds) const;
		bool operator!=(const Bounds& bounds) const;

	};

protected:
	class Vertex {

	public:
		explicit Vertex(vec2 position) :
				_position(position), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(0.0f, 0.0f) {}

		Vertex(vec2 position, vec4 color) :
				_position(position), _color(color), _texture(0.0f, 0.0f) {}

		Vertex(vec2 position, vec2 texture) :
				_position(position), _color(1.0f, 1.0f, 1.0f, 1.0f), _texture(texture) {}

	private:
		vec2 _position;
		vec4 _color;
		vec2 _texture;

	};

public:
	~Element() override = default;

	Element(Element&&) = default;
	Element& operator=(Element&&) = default;

	/**
	 * Sets the default width and height of this Element.
	 */
	void SetDefaultSize(unsigned width, unsigned height);

	/**
	 * Sets the focusable flag of this element. If this element is currently in
	 * focus, calling this element does not make this element lose its focus.
	 */
	void SetFocusable(bool focusable);

	/**
	 * Returns whether this element is focusable.
	 */
	bool IsFocusable() const;

	/**
	 * Returns whether this element has focus.
	 */
	bool HasFocus() const;

	/**
	 * Requests that this element get the focus from the UI. Returns true if the
	 * request resulted in this element receiving the focus.
	 */
	bool RequestFocus();

	/**
	 * If this element is currently focused, calling this method will remove the
	 * focus from this element. Returns true if the element was focused before
	 * calling this focus.
	 */
	bool LoseFocus();

	/**
	 * Return true if this element should receive mouse events. If this method
	 * returns false, the element is transparent to mouse events.
	 */
	virtual bool IsOpaque();

	/**
	 * Sets the drag-enabled flag for this element. If this is set to true, the
	 * OnMouseDrag() method will be called when the mouse is moved while a
	 * button is pressed.
	 */
	void SetDragEnabled(bool drag);

	/**
	 * Returns whether the drag is enabled for this element.
	 */
	bool IsDragEnabled() const;

	/**
	 * Set the bounds of this Element, in pixel space. If this element is a
	 * child of a Container, this method is called when the parent container is
	 * layed-out.
	 */
	void SetBounds(Bounds bounds);

	/**
	 * Returns the bounds of this element, in Normalized Device Coordinates.
	 */
	const Bounds& GetBounds() const;

	/**
	 * Returns the bounds of this element, in Normalized Device Coordinates.
	 */
	Bounds& GetBounds();

	/**
	 * Initializes the bounds of this elements. Calling this method after it was
	 * already called, or when SetBounds was called, has no effect.
	 */
	void InitializeBounds();

	/**
	 * Returns the root container of this element.
	 */
	const Container* RootContainer() const;

	/**
	 * Returns the root container of this element.
	 */
	Container* RootContainer();

	/**
	 * Draws the element. Override DoDraw(float, float) for custom drawing.
	 */
	void Draw(float time, float dt) const;

	/**
	 * Adds an input callback. When an input event occurs, this callback object
	 * will also receive the event. Events are received in the same order as
	 * callbacks are added.
	 */
	void AddInputCallback(std::shared_ptr<InputCallback> callback);

	/**
	 * Removes an input callback. Removed input callbacks will no longer receive
	 * input events.
	 */
	void RemoveInputCallback(const std::shared_ptr<InputCallback>& callback);

protected:
	Element();

	/**
	 * Moves the fields of this Element superclass when moving derived objects.
	 */
	void MoveSuperFields(Element&& element);

private:
	void _callback(const std::function<void(const CbPtr&)>& callback);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-virtual-near-miss"
	void _on_resize();
	void _on_focus_gained();
	void _on_focus_lost();
	void _on_key_press(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _on_key_repeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _on_key_release(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void _on_character_input(Input::Unicode character);
	void _on_mouse_button_press(Input::MouseButton button, Input::Modifiers mods);
	void _on_mouse_button_release(Input::MouseButton button, Input::Modifiers mods);
	void _on_mouse_enter(const vec2& position);
	void _on_mouse_exit(const vec2& position);
	void _on_mouse_move(const vec2& position);
	void _on_mouse_jump(const vec2& position);
	void _on_mouse_drag(const vec2& origin, const vec2& position);
	void _on_mouse_scroll(const vec2& scroll_components);
#pragma clang diagnostic pop

	Container* _parent_container;
	Bounds _bounds{};
	bool _has_initialized_bounds = false;

	unsigned _default_width = 20;
	unsigned _default_height = 20;
	bool _focusable = false;
	bool _drag_enabled = false;

	std::vector<CbPtr> _callback_list;

protected:
	/**
	 * Draws this UI element.
	 */
	virtual void DoDraw(float time, float dt) const = 0;

	/**
	 * Returns the animator for this element. If the element doesn't have an
	 * animator, it will be created.
	 */
	Animator& GetAnimator();

	/**
	 * Returns whether this element has an animator.
	 */
	bool HasAnimator() const;

	/**
	 * Returns the text renderer.
	 */
	TextRenderer& GetTextRenderer() const;

	/**
	 * Draws a colored triangle. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is recommended.
	 */
	void DrawColoredTriangle(const Vertex& v_1, const Vertex& v_2, const Vertex& v_3) const;

	/**
	 * Draws a colored quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	void DrawColoredQuad(const Vertex& v_1, const Vertex& v_2, const Vertex& v_3, const Vertex& v_4) const;

	/**
	 * Draws a rectangle with a single color.
	 */
	void DrawColoredQuad(const Bounds& bounds, const Color& color) const;

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is recommended.
	 */
	void DrawTexturedTriangle(const Vertex& v_1, const Vertex& v_2, const Vertex& v_3, const gl::Texture2D& texture) const;

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	void DrawTexturedQuad(const Vertex& v_1, const Vertex& v_2, const Vertex& v_3, const Vertex& v_4, const gl::Texture2D& texture) const;

	/**
	 * Draws the texture at the specified rectangle. The entire texture will be
	 * scaled to fit the rectangle.
	 */
	void DrawTexturedQuad(const Bounds& bounds, const gl::Texture2D& texture) const;

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	void DrawTexturedQuad(const Vertex& v_1, const Vertex& v_2, const Vertex& v_3, const Vertex& v_4, const Image& image, float alpha = 1.0f) const;

	/**
	 * Draws the texture at the specified rectangle. The entire texture will be
	 * scaled to fit the rectangle.
	 */
	void DrawTexturedQuad(const Bounds& bounds, const Image& image, float alpha = 1.0f) const;

	/**
	 * Draws a rectangle using the shader.
	 */
	void DrawShaderedQuad(const Bounds& bounds, const Shader& shader) const;

	/**
	 * Returns the custom shader program from the given shader asset. This
	 * instance can be used to set uniform variables.
	 */
	const gl::Program& GetCustomShader(const Shader& shader) const;

private:
	void _draw(const std::vector<Vertex>& vertices, int mode, float alpha = 1.0f) const;

	mutable std::unique_ptr<Animator> _animator;

};

}

#endif
