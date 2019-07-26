#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
#include "../../_common.h"

#include "Font.h"

namespace rheel {

class RE_API TextRenderer {
	RE_NO_CONSTRUCT(TextRenderer)

public:
	static void DrawText(Font& font, const std::wstring& text,
			int x, int y, unsigned size);

	static void DrawText(Font& font, const std::string& text,
			int x, int y, unsigned size);

private:
	static int _DrawChars(Font& font, const wchar_t *text, unsigned length,
			int x, int y, unsigned size);

};

}

#endif
