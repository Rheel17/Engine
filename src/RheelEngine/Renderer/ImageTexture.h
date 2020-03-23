/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGETEXTURE_H_
#define IMAGETEXTURE_H_
#include "../_common.h"

#include "../Assets/Image.h"
#include "_OpenGL/_GLTexture2D.h"

namespace rheel {

class RE_API ImageTexture {

public:
	void Bind(unsigned textureUnit) const;

private:
	explicit ImageTexture(const Image& image);

	_GLTexture2D _texture;

public:
	static const ImageTexture& Get(const Image& image);

private:
	static std::unordered_map<std::uintptr_t, ImageTexture> _texture_cache;

};

}

#endif
