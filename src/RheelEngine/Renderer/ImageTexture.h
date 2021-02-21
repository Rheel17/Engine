/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_IMAGETEXTURE_H
#define RHEELENGINE_IMAGETEXTURE_H
#include "../_common.h"

#include "../Assets/Image.h"
#include "../Util/Cache.h"
#include "OpenGL/Texture2D.h"

namespace rheel {

class RE_API ImageTexture {

public:
	enum class WrapType {
		WRAP, CLAMP
	};

private:
	using CacheTuple = std::tuple<uintptr_t, WrapType, bool>;

public:
	void Bind(unsigned texture_unit) const;

private:
	ImageTexture(const Image& image, WrapType type, bool linear);

	gl::Texture2D _texture;

public:
	static const ImageTexture& Get(const Image& image, WrapType type = WrapType::WRAP, bool linear = true);

private:
	static Cache<CacheTuple, ImageTexture, keep_policy, true> _texture_cache;

};

}

namespace std {

template<>
struct hash<std::tuple<std::uintptr_t, rheel::ImageTexture::WrapType, bool>> {
	std::size_t operator()(const std::tuple<std::uintptr_t, rheel::ImageTexture::WrapType, bool>& tpl) const {
		const auto&[a, t, l] = tpl;
		return (a + 31 * int(t)) * 971 + l;
	}
};

}

#endif
