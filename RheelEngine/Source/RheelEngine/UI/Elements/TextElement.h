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
	TextElement(std::string text, unsigned size);
	TextElement(std::wstring text, unsigned size);
	TextElement(std::string text, Font& font, unsigned size);
	TextElement(std::wstring text, Font& font, unsigned size);

	std::string _text;
	std::wstring _w_text;
	bool _wide;

	Font& _font;

	unsigned _font_ascend;
	unsigned _font_descend;
	unsigned _size;

public:
	/**
	 * Create a text component with text and size. The default font is used.
	 */
	inline static std::shared_ptr<TextElement> Create(std::string text, unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(text, size));
	}

	/**
	 * Create a text component with wide text (allowing Unicode characters) and
	 * size. The default font is used.
	 */
	inline static std::shared_ptr<TextElement> Create(std::wstring text, unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(text, size));
	}

	/**
	 * Create a text component with text, font, and size.
	 */
	inline static std::shared_ptr<TextElement> Create(std::string text, Font& font, unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(text, font, size));
	}

	/**
	 * Create a text component with wide text (allowing Unicode characters),
	 * font, and size.
	 */
	inline static std::shared_ptr<TextElement> Create(std::wstring text, Font& font, unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(text, font, size));
	}
};

}

#endif
