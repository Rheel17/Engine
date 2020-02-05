/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGETEXTURE_H_
#define IMAGETEXTURE_H_
#include "../_common.h"

#include "../Assets/Image.h"
#include "OpenGL/GLTexture2D.h"

namespace rheel {

class RE_API ImageTexture {

public:
	void Bind(unsigned textureUnit) const;

private:
	explicit ImageTexture(Image image);

	GLTexture2D _texture;

public:
	static ImageTexture& GetFor(Image image);

private:
	static std::unordered_map<Image, ImageTexture> _texture_cache;

};

}

#endif
