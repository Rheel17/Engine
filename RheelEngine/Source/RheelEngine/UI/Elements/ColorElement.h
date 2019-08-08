#ifndef COLORELEMENT_H_
#define COLORELEMENT_H_
#include "../../_common.h"

#include "../Element.h"
#include "../../Renderer/OpenGL/GL.h"

namespace rheel {

class RE_API ColorElement : public Element {
	friend class UI;

public:
	/**
	 * Create a color component with a certain color, with its size initialized to 0.
	 */
	ColorElement(Color color);

	/**
	 * Create a color component of a certain width and height, and with a certain color
	 */
	ColorElement(unsigned width, unsigned height, Color color);

	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override;

private:
	unsigned _width, _height;
	Color _color;

};

}

#endif
