#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_
#include "../../_common.h"

#include "Font.h"
#include "../OpenGL/GLVertexArray.h"
#include "../OpenGL/GLShaderProgram.h"

namespace rheel {

class RE_API TextRenderer {
	RE_NO_CONSTRUCT(TextRenderer)

public:
	static void DrawText(Font& font, const std::wstring& text,
			int x, int y, unsigned size);

	static void DrawText(Font& font, const std::string& text,
			int x, int y, unsigned size);

private:
	static void _Initialize();

	static int _DrawChars(Font& font, const wchar_t *text, unsigned length,
			int x, int y, unsigned size);

	static std::shared_ptr<GLBuffer> _triangle_buffer;
	static std::shared_ptr<GLVertexArray> _vao;
	static std::shared_ptr<GLBuffer> _screenquad_vbo;
	static std::shared_ptr<GLVertexArray> _screenquad_vao;
	static GLShaderProgram _shader;
	static bool _initialized;

};

}

#endif
