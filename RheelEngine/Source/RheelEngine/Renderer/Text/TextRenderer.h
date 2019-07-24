#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
#include "../../_common.h"

#include "Font.h"
#include "../OpenGL/GLVertexArray.h"
#include "../OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API TextRenderer {

	struct _StaticData {
		~_StaticData();
		void Initialize();

		bool initialized = false;
		GLBuffer *vbo;
		GLBuffer *eab;
		GLVertexArray *vao;
		GLShaderProgram *shader;
	};

	struct Character {
		ivec4 texture_data;
		vec4 character_data;
	};

public:
	static void DrawText(Font& font, const std::wstring& text,
			int x, int y, unsigned size);

	static void DrawText(Font& font, const std::string& text,
			int x, int y, unsigned size);

private:
	static void _DrawChars(Font& font, const wchar_t *text, unsigned length,
			int x, int y, unsigned size);

	static _StaticData _gl;

};

}

#endif
