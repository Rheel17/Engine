/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ELEMENT_H
#define RHEELENGINE_ELEMENT_H
#include "../_common.h"

#include <type_traits>

#include "InputCallback.h"
#include "../Color.h"
#include "../Assets/Image.h"
#include "../Assets/Shader.h"
#include "../Renderer/OpenGL/Program.h"
#include "../Renderer/OpenGL/VertexArray.h"
#include "../Renderer/OpenGL/Texture2D.h"

namespace rheel {

class Container;
class UI;

class RE_API Element : public InputCallback {
	friend class UI;
	friend class Container;

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

	/**
	 * Draws this UI element.
	 */
	virtual void Draw(float time, float dt) const = 0;

protected:
	Element();

	/**
	 * Moves the fields of this Element superclass when moving derived objects.
	 */
	void MoveSuperFields(Element&& element);

private:
	void Callback_(const std::function<void(const CbPtr&)>& callback);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-virtual-near-miss"
	void OnResize_();
	void OnFocusGained_();
	void OnFocusLost_();
	void OnKeyPress_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void OnKeyRepeat_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void OnKeyRelease_(Input::Key key, Input::Scancode scancode, Input::Modifiers mods);
	void OnCharacterInput_(Input::Unicode character);
	void OnMouseButtonPress_(Input::MouseButton button, Input::Modifiers mods);
	void OnMouseButtonRelease_(Input::MouseButton button, Input::Modifiers mods);
	void OnMouseEnter_(const vec2& position);
	void OnMouseExit_(const vec2& position);
	void OnMouseMove_(const vec2& position);
	void OnMouseJump_(const vec2& position);
	void OnMouseDrag_(const vec2& origin, const vec2& position);
	void OnMouseScroll_(const vec2& scrollComponents);
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
	 * Draws a colored triangle. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is recommended.
	 */
	static void DrawColoredTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	/**
	 * Draws a colored quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	static void DrawColoredQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

	/**
	 * Draws a rectangle with a single color.
	 */
	static void DrawColoredQuad(const Bounds& bounds, const Color& color);

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is recommended.
	 */
	static void DrawTexturedTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const gl::Texture2D& texture);

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	static void DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const gl::Texture2D& texture);

	/**
	 * Draws the texture at the specified rectangle. The entire texture will be
	 * scaled to fit the rectangle.
	 */
	static void DrawTexturedQuad(const Bounds& bounds, const gl::Texture2D& texture);

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space.
	 * Counter-clockwise ordering is required.
	 */
	static void DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const Image& image,	float alpha = 1.0f);

	/**
	 * Draws the texture at the specified rectangle. The entire texture will be
	 * scaled to fit the rectangle.
	 */
	static void DrawTexturedQuad(const Bounds& bounds, const Image& image, float alpha = 1.0f);

	/**
	 * Draws a rectangle using the shader.
	 */
	static void DrawShaderedQuad(const Bounds& bounds, const Shader& shader);

	/**
	 * Returns the custom shader program from the given shader asset. This
	 * instance can be used to set uniform variables.
	 */
	static const gl::Program& GetCustomShader(const Shader& shader);

private:
	static void Draw_(const std::vector<Vertex>& vertices, int mode, float alpha = 1.0f);
	static void Initialize_();

	static std::unique_ptr<gl::Program> _ui_shader;
	static std::unique_ptr<gl::VertexArray> _ui_vao;
	static std::unique_ptr<gl::Buffer> _ui_vertex_data;
	static bool _initialized;

	static std::unordered_map<std::uintptr_t, gl::Program> _custom_shaders;

};

}

#endif
