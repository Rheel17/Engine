/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGETEXTURE_H_
#define IMAGETEXTURE_H_
#include "../_common.h"

#include "../Resources/Image.h"
#include "OpenGL/GLTexture2D.h"

namespace rheel {

class RE_API ImageTexture {

public:
	ImageTexture(const Image& image);

	void Bind(unsigned textureUnit) const;

private:
	GLTexture2D _texture;

};

}

#endif
