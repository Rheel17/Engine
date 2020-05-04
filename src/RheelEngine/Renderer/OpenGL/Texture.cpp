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

std::ostream& operator<<(std::ostream& out, Texture::Target target) {
	switch (target) {
		case Texture::Target::TEXTURE_1D:
			return out << "TEXTURE_1D";
		case Texture::Target::TEXTURE_2D:
			return out << "TEXTURE_2D";
		case Texture::Target::TEXTURE_3D:
			return out << "TEXTURE_3D";
		case Texture::Target::TEXTURE_1D_ARRAY:
			return out << "TEXTURE_1D_ARRAY";
		case Texture::Target::TEXTURE_2D_ARRAY:
			return out << "TEXTURE_2D_ARRAY";
		case Texture::Target::TEXTURE_RECTANGLE:
			return out << "TEXTURE_RECTANGLE";
		case Texture::Target::TEXTURE_CUBE_MAP:
			return out << "TEXTURE_CUBE_MAP";
		case Texture::Target::TEXTURE_CUBE_MAP_ARRAY:
			return out << "TEXTURE_CUBE_MAP_ARRAY";
		case Texture::Target::TEXTURE_BUFFER:
			return out << "TEXTURE_BUFFER";
		case Texture::Target::TEXTURE_2D_MULTISAMPLE:
			return out << "TEXTURE_2D_MULTISAMPLE";
		case Texture::Target::TEXTURE_2D_MULTISAMPLE_ARRAY:
			return out << "TEXTURE_2D_MULTISAMPLE_ARRAY";
	}

	return out;
}

}
