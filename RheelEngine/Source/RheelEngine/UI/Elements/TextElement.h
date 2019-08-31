#ifndef TEXTELEMENT_H_
#define TEXTELEMENT_H_
#include "../../_common.h"

#include "../Element.h"
#include "../../Renderer/Text/Font.h"

namespace rheel {

class RE_API TextElement : public Element {

public:
	/**
	 * Create a text component with text and size. The default font is used.
	 */
	TextElement(std::string text, unsigned size);

	/**
	 * Create a text component with wide text (allowing Unicode characters) and
	 * size. The default font is used.
	 */
	TextElement(std::wstring text, unsigned size);

	/**
	 * Create a text component with text, font, and size.
	 */
	TextElement(std::string text, Font& font, unsigned size);

	/**
	 * Create a text component with wide text (allowing Unicode characters),
	 * font, and size.
	 */
	TextElement(std::wstring text, Font& font, unsigned size);

	/**
	 * Sets the text displayed by this element
	 */
	void SetText(std::string text);

	/**
	 * Sets the text displayed by this element
	 */
	void SetText(std::wstring text);

	void Draw(float dt) const override;


private:
	std::string _text;
	std::wstring _w_text;
	bool _wide;

	Font& _font;

	unsigned _font_ascend;
	unsigned _font_descend;
	unsigned _size;

};

}

#endif
