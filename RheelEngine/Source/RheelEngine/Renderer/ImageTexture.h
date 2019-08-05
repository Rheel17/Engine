#ifndef IMAGETEXTURE_H_
#define IMAGETEXTURE_H_
#include "../_common.h"

#include <unordered_map>

#include "../Image.h"
#include "OpenGL/GLTexture2D.h"

namespace rheel {

class RE_API ImageTexture {
	friend class Image;

public:
	ImageTexture(Image *image);
	void Bind(unsigned textureUnit) const;

private:
	GLTexture2D _texture;

public:
	static const ImageTexture& Get(Image *image);

private:
	static void _DestroyTexture(Image *image);

	static std::unordered_map<Image *, ImageTexture> _texture_map;

};

}

#endif
