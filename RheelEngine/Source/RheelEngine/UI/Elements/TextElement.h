#ifndef TEXTELEMENT_H_
#define TEXTELEMENT_H_
#include "../../_common.h"
#include "../../Renderer/Text/Font.h"

#include "../Element.h"

namespace rheel {

class RE_API TextElement : public Element {

public:
	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override;

private:
	TextElement(unsigned size);
	TextElement(Font& font, unsigned size);

	Font& _font;
	unsigned _font_ascend;
	unsigned _font_descend;
	unsigned _size;

public:
	/**
	 * Create a text component of a certain width and height.
	 */
	inline static std::shared_ptr<TextElement> Create(unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(size));
	}

};

}

#endif
