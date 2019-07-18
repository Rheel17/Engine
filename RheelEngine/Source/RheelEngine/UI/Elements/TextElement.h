#ifndef TEXTELEMENT_H_
#define TEXTELEMENT_H_
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API TextElement : public Element {

public:
	std::pair<unsigned, unsigned> GetDefaultDimensions() const override;
	void Draw() const override;

private:
	TextElement(unsigned size);

	unsigned _size;

public:
	/**
	 * Create a color component of a certain width and height, and with a certain color
	 */
	inline static std::shared_ptr<TextElement> Create(unsigned size) {
		// not std::make_shared<...>(...) because than the constructor can't be private.
		return std::shared_ptr<TextElement>(new TextElement(size));
	}

};

}

#endif
