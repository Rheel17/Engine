#include "Material.h"

#include "Resources.h"
#include "Renderer/ImageTexture.h"

namespace rheel {

Material::_ImagePtr Material::UV_TEST_TEXTURE = std::make_shared<Image>(
		Resources::Start("Textures_uv_grid_png"),
		Resources::Length("Textures_uv_grid_png"),
		Image::FormatPNG);

const Material Material::UV_TEST_MATERIAL(UV_TEST_TEXTURE, 0.0f, 0.0f);

Material::Material() :
		Material(Color { 1.0f, 1.0f, 1.0f, 1.0f }, 0.9f, 0.3f) {}

Material::Material(Color color, float diffuse, float specular, float specularExponent) :
		_type(Colored), _color(std::move(color)), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(_ImagePtr texture, float diffuse, float specular, float specularExponent) :
		_type(Textured), _color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(texture), _diffuse_texture(texture),
		_specular_texture(texture), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(_ImagePtr ambientTexture, _ImagePtr diffuseTexture, _ImagePtr specularTexture, float specularExponent) :
		Material(ambientTexture, diffuseTexture, specularTexture, 1.0f, 1.0f, 1.0f, specularExponent) {}

Material::Material(_ImagePtr ambientTexture, _ImagePtr diffuseTexture, _ImagePtr specularTexture, float ambientFactor, float diffuseFactor, float specularFactor, float specularExponent) :
		_type(Textured), _color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(ambientTexture), _diffuse_texture(diffuseTexture),
		_specular_texture(specularTexture), _ambient_factor(ambientFactor),
		_diffuse_factor(diffuseFactor), _specular_factor(specularFactor),
		_specular_exponent(specularExponent) {}

Material::MaterialType Material::Type() const {
	return _type;
}

vec4 Material::MaterialVector() const {
	return { _ambient_factor, _diffuse_factor, _specular_factor, _specular_exponent };
}

const Color& Material::MaterialColor() const {
	return _color;
}

void Material::BindTextures() const {
	ImageTexture::Get(_ambient_texture.get()).Bind(0);
	ImageTexture::Get(_diffuse_texture.get()).Bind(1);
	ImageTexture::Get(_specular_texture.get()).Bind(2);
}

const Material::_ImagePtr Material::AmbientTexture() const {
	return _ambient_texture;
}

const Material::_ImagePtr Material::DiffuseTexture() const {
	return _diffuse_texture;
}

const Material::_ImagePtr Material::SpecularTexture() const {
	return _specular_texture;
}

}
