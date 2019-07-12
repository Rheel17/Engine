#include "Material.h"

#include <cmath>

namespace rheel {

Material::Material() :
		Material(Color { 1.0f, 1.0f, 1.0f, 1.0f }, 0.9f, 0.3f) {}

Material::Material(Color color, float diffuse, float specular, float specularExponent) :
		_type(Colored), _color(std::move(color)), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(ImagePtr texture, float diffuse, float specular, float specularExponent) :
		_type(Textured), _ambient_texture(texture), _diffuse_texture(texture),
		_specular_texture(texture), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(ImagePtr ambientTexture, ImagePtr diffuseTexture, ImagePtr specularTexture, float specularExponent) :
		Material(ambientTexture, diffuseTexture, specularTexture, 1.0f, 1.0f, 1.0f, specularExponent) {}

Material::Material(ImagePtr ambientTexture, ImagePtr diffuseTexture, ImagePtr specularTexture, float ambientFactor, float diffuseFactor, float specularFactor, float specularExponent) :
		_type(Textured), _ambient_texture(ambientTexture),
		_diffuse_texture(diffuseTexture), _specular_texture(specularTexture),
		_ambient_factor(ambientFactor), _diffuse_factor(diffuseFactor),
		_specular_factor(specularFactor), _specular_exponent(specularExponent) {}

vec4 Material::MaterialVector() const {
	return { _ambient_factor, _diffuse_factor, _specular_factor, _specular_exponent };
}

const Color& Material::MaterialColor() const {
	return _color;
}

}
