#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
#include "../../_common.h"

#include "Font.h"

namespace rheel {

class RE_API TextRenderer {

public:
	static void DrawText(Font& font, const std::wstring& text,
			unsigned x, unsigned y, unsigned size);

	static void DrawText(Font& font, const std::string& text,
			unsigned x, unsigned y, unsigned size);

private:
	static void _DrawChars(Font& font, const wchar_t *text, unsigned length,
			unsigned x, unsigned y, unsigned size);

};

}

#endif
