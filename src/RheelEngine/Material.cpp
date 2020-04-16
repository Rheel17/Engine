/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Material.h"

#include <utility>

#include "Renderer/ImageTexture.h"

namespace rheel {

Material::Material() :
		Material(Color{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.9f, 0.3f) {}

Material::Material(Color color, float diffuse, float specular, float specularExponent) :
		_type(COLORED),
		_color(color),
		_specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(const Image& texture, float diffuse, float specular, float specularExponent) :
		_type(TEXTURED),
		_color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(texture),
		_diffuse_texture(texture),
		_specular_texture(texture),
		_specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(Image ambientTexture, Image diffuseTexture, Image specularTexture, float specularExponent) :
		Material(std::move(ambientTexture), std::move(diffuseTexture), std::move(specularTexture), 1.0f, 1.0f, 1.0f, specularExponent) {}

Material::Material(Image ambientTexture,
		Image diffuseTexture,
		Image specularTexture,
		float ambientFactor,
		float diffuseFactor,
		float specularFactor,
		float specularExponent) :
		_type(TEXTURED),
		_color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(std::move(ambientTexture)),
		_diffuse_texture(std::move(diffuseTexture)),
		_specular_texture(std::move(specularTexture)),
		_ambient_factor(ambientFactor),
		_diffuse_factor(diffuseFactor),
		_specular_factor(specularFactor),
		_specular_exponent(specularExponent) {}

Material::Material(Shader fragmentShader) :
		_type(CUSTOM_SHADER),
		_custom_shader(std::move(fragmentShader)) {}

Material::MaterialType Material::Type() const {
	return _type;
}

vec4 Material::MaterialVector() const {
	return { _ambient_factor, _diffuse_factor, _specular_factor, _specular_exponent };
}

const Color& Material::GetColor() const {
	return _color;
}

float Material::AmbientFactor() const {
	return _ambient_factor;
}

float Material::DiffuseFactor() const {
	return _diffuse_factor;
}

float Material::SpecularFactor() const {
	return _specular_factor;
}

void Material::BindTextures() const {
	ImageTexture::Get(_ambient_texture).Bind(0);
	ImageTexture::Get(_diffuse_texture).Bind(1);
	ImageTexture::Get(_specular_texture).Bind(2);
}

Image Material::GetAmbientTexture() const {
	return _ambient_texture;
}

Image Material::GetDiffuseTexture() const {
	return _diffuse_texture;
}

Image Material::GetSpecularTexture() const {
	return _specular_texture;
}

Shader Material::GetCustomShader() const {
	return _custom_shader;
}

}