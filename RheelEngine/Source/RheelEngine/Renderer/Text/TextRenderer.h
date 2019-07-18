#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_

#include "Font.h"

namespace rheel {

class TextRenderer {

public:
	static void DrawText(Font& font, const std::string& text,
			unsigned x, unsigned y, unsigned size);

};

}

#endif
