/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture.h"

#include "Context.h"

namespace rheel::gl {

Texture::Texture(Texture::Target target) :
		_target(target) {}

Texture::Target rheel::gl::Texture::GetTarget() const {
	return _target;
}

void Texture::Bind(unsigned int textureUnit) const {
	Context::Current().BindTexture(textureUnit, *this);
}

}
