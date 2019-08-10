#ifndef ELEMENT_H_
#define ELEMENT_H_
#include "../_common.h"

#include <memory>
#include <type_traits>

#include "../Renderer/OpenGL/GLShaderProgram.h"
#include "../Renderer/OpenGL/GLVertexArray.h"
#include "../Renderer/OpenGL/GLTexture2D.h"

#include "../Color.h"

#define __ELEMENT__													\
	inline Element *_Clone() const override {						\
		return new (std::remove_const<								\
					std::remove_pointer<decltype(this)>::type		\
				>::type)(*this);									\
	}

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
	 * Draws this UI element.
	 */
	virtual void Draw() const = 0;

protected:
	Element();

	/**
	 * Copies the fields of this Element superclass when copying derived
	 * objects.
	 */
	void _CopySuperFields(const Element& element);

private:
	virtual Element *_Clone() const = 0;

	Container *_parent_container;
	Bounds _bounds;
	bool _has_initialized_bounds = false;

	unsigned _default_width = 20;
	unsigned _default_height = 20;

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

using ElementPtr = std::shared_ptr<Element>;

}

#endif
