#ifndef ELEMENT_H_
#define ELEMENT_H_
#include "../_common.h"

#include <memory>
#include <type_traits>

#include "Input.h"
#include "../Color.h"
#include "../Renderer/OpenGL/GLShaderProgram.h"
#include "../Renderer/OpenGL/GLVertexArray.h"
#include "../Renderer/OpenGL/GLTexture2D.h"

namespace rheel {

class UI;
class Container;

class RE_API Element {
	friend class UI;
	friend class Container;

public:
	struct Bounds {
		unsigned x, y;
		unsigned width, height;

		bool operator==(const Bounds& bounds) const;
		bool operator!=(const Bounds& bounds) const;
	};

protected:
	class Vertex {

	public:
		Vertex(vec2 position);
		Vertex(vec2 position, vec4 color);
		Vertex(vec2 position, vec2 texture);

	private:
		vec2 _position;
		vec4 _color;
		vec2 _texture;

	};

public:
	virtual ~Element() = default;

	/**
	 * Sets the default width and height of this Element.
	 */
	void SetDefaultSize(unsigned width, unsigned height);

	/**
	 * Sets the focusable flag of this element.
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
	const Container *RootContainer() const;

	/**
	 * Returns the root container of this element.
	 */
	Container *RootContainer();

	/**
	 * Draws this UI element.
	 */
	virtual void Draw() const = 0;

	/**
	 * Called when this element has been resized.
	 */
	virtual void OnResize() {};

	/**
	 * Called when this element receives the focus in the UI.
	 */
	virtual void OnFocusGained() {};

	/**
	 * Called when the focus of this element is lost. This can happen because
	 * another element receives focus, or the parent window itself looses focus.
	 */
	virtual void OnFocusLost() {};

	/**
	 * Called when a key on the keyboard is pressed down.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyPress(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {};

	/**
	 * Called when a key on the keyboard is kept pressed down long enough to
	 * trigger a repeat.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyRepeat(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {};

	/**
	 * Called when a key on the keyboard is kept released.
	 *
	 * Parameters
	 * 	key: the code of the key
	 * 	scancode: the location of the key on the keyboard
	 * 	mods: any modifiers that were used
	 */
	virtual void OnKeyRelease(Input::Key key, Input::Scancode scancode, Input::Modifiers mods) {};

	/**
	 * Called when a text character is inputed.
	 *
	 * Parameters
	 * 	character: the unicode character that was inputed
	 */
	virtual void OnCharacterInput(wchar_t character) {};

	/**
	 * Called when a mouse button is pressed down.
	 *
	 * Parameters
	 * 	button: the button that was pressed
	 * 	mods: any modifiers that were used
	 */
	virtual void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) {};

	/**
	 * Called when a mouse button is released.
	 *
	 * Parameters
	 * 	button: the button that was pressed
	 * 	mods: any modifiers that were used
	 */
	virtual void OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) {};

	/**
	 * Called when the mouse enters the bounds of this component.
	 *
	 * x: the x-coordinate of the mouse position
	 * y: the y-coordinate of the mouse position
	 */
	virtual void OnMouseEnter(float x, float y) {}

	/**
	 * Called when the mouse exits the bounds of this component.
	 *
	 * x: the x-coordinate of the mouse position
	 * y: the y-coordinate of the mouse position
	 */
	virtual void OnMouseExit(float x, float y) {}

	/**
	 * Called when the mouse is moved without any buttons pressed.
	 *
	 * Parameters
	 * 	x: the x-coordinate of the new mouse position
	 * 	y: the y-coordinate of the new mouse position
	 */
	virtual void OnMouseMove(float x, float y) {};

	/**
	 * Called when the mouse is moved with at least one button pressed.
	 *
	 * Parameters
	 * 	xOrigin: the x-coordinate of the origin of the drag (where the mouse was
	 * 		clicked)
	 * 	yOrigin: the y-coordinate of the origin of the drag (where the mouse was
	 * 		clicked)
	 * 	x: the x-coordinate of the current mouse position
	 * 	y: the y-coordinate of the current mouse position
	 */
	virtual void OnMouseDrag(float xOrigin, float yOrigin, float x, float y) {};

	/**
	 * Called when the mouse is scrolled.
	 *
	 * Parameters
	 * 	x: the x component of the scroll
	 * 	y: the y component of the scroll
	 */
	virtual void OnMouseScroll(float x, float y) {};

protected:
	Element();

	/**
	 * Moves the fields of this Element superclass when moving derived objects.
	 */
	void _MoveSuperFields(Element&& element);

private:
	Container *_parent_container;
	Bounds _bounds;
	bool _has_initialized_bounds = false;

	unsigned _default_width = 20;
	unsigned _default_height = 20;
	bool _focusable = false;

protected:
	/**
	 * Draws a colored triangle. Vertices are specified in pixel-space. Counter-clockwise
	 * ordering is recommended.
	 */
	static void _DrawColoredTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	/**
	 * Draws a colored quad. Vertices are specified in pixel-space. Counter-clockwise
	 * ordering is required.
	 */
	static void _DrawColoredQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

	/**
	 * Draws a rectangle with a single color.
	 */
	static void _DrawColoredQuad(const Bounds& bounds, const Color& color);

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space. Counter-clockwise
	 * ordering is recommended.
	 */
	static void _DrawTexturedTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const GLTexture2D& texture);

	/**
	 * Draws a textured quad. Vertices are specified in pixel-space. Coutner-clockwise
	 * ordering is required.
	 */
	static void _DrawTexturedQuad(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4, const GLTexture2D& texture);

	/**
	 * Draws the texture at the specified rectangle. The entire texture will be scaled to
	 * fit the rectangle.
	 */
	static void _DrawTexturedQuad(const Bounds& bounds, const GLTexture2D& texture);

private:
	static void _Draw(const std::vector<Vertex>& vertices, int mode);
	static void _Initialize();

	static std::shared_ptr<GLShaderProgram> _ui_shader;
	static std::shared_ptr<GLVertexArray> _ui_vao;
	static std::shared_ptr<GLBuffer> _ui_vertex_data;
	static bool _initialized;

};

}

#endif
