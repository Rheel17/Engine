#ifndef COLORELEMENT_H_
#define COLORELEMENT_H_
#include "../../_common.h"

#include "../Element.h"
#include "../../Renderer/OpenGL/GL.h"

namespace rheel::ui {

class RE_API ColorElement : public Element {

public:
	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override;

private:
	ColorElement(unsigned width, unsigned height, Color&& color);

	unsigned _width, _height;
	Color _color;

public:
	/**
	 * Create a color component with a certain color, with its size initialized to 0.
	 */
	inline static std::shared_ptr<ColorElement> Create(Color color) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<ColorElement>(new ColorElement(0, 0, std::move(color)));
	}

	/**
	 * Create a color component of a certain width and height, and with a certain color
	 */
	inline static std::shared_ptr<ColorElement> Create(unsigned width, unsigned height, Color color) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<ColorElement>(new ColorElement(width, height, std::move(color)));
	}

};

}

#endif
