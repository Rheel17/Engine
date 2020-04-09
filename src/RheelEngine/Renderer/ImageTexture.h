/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGETEXTURE_H_
#define IMAGETEXTURE_H_
#include "../_common.h"

#include "../Assets/Image.h"
#include "OpenGL/Texture2D.h"

namespace rheel {

class RE_API ImageTexture {

public:
	enum class WrapType {
		WRAP, CLAMP
	};

public:
	void Bind(unsigned textureUnit) const;

private:
	ImageTexture(const Image& image, WrapType type, bool linear);

	GL::Texture2D _texture;

public:
	static const ImageTexture& Get(const Image& image, WrapType type = WrapType::WRAP, bool linear = true);

private:
	static std::unordered_map<std::tuple<uintptr_t, WrapType, bool>, ImageTexture> _texture_cache;

};

}

namespace std {

template<>
struct hash<std::tuple<std::uintptr_t, rheel::ImageTexture::WrapType, bool>> {
	std::size_t operator()(const std::tuple<std::uintptr_t, rheel::ImageTexture::WrapType, bool>& tpl) const {
		const auto& [a, t, l] = tpl;
		return (a + 31 * int(t)) * 971 + l;
	}
};

}

#endif
