/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Texture.h"

#include "State.h"

namespace rheel::gl {

Texture::Texture(Texture::Target target) :
		_target(target) {}

Texture::Target rheel::gl::Texture::GetTarget() const {
	return _target;
}

void Texture::Bind(unsigned int textureUnit) const {
	State::BindTexture(textureUnit, *this);
}

}
