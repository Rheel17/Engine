/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Material.h"

#include "Renderer/ImageTexture.h"

namespace rheel {

Material::Material() :
		Material(Color{ 1.0f, 1.0f, 1.0f, 1.0f }, 0.9f, 0.3f) {}

Material::Material(Color color, float diffuse, float specular, float specular_exponent) :
		_type(COLORED),
		_color(color),
		_specular_exponent(specular_exponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(const Image& texture, float diffuse, float specular, float specular_exponent) :
		_type(TEXTURED),
		_color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(texture),
		_diffuse_texture(texture),
		_specular_texture(texture),
		_specular_exponent(specular_exponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(Image ambient_texture, Image diffuse_texture, Image specular_texture, float specular_exponent) :
		Material(std::move(ambient_texture), std::move(diffuse_texture), std::move(specular_texture), 1.0f, 1.0f, 1.0f, specular_exponent) {}

Material::Material(Image ambient_texture,
		Image diffuse_texture,
		Image specular_texture,
		float ambient_factor,
		float diffuse_factor,
		float specular_factor,
		float specular_exponent) :
		_type(TEXTURED),
		_color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(std::move(ambient_texture)),
		_diffuse_texture(std::move(diffuse_texture)),
		_specular_texture(std::move(specular_texture)),
		_ambient_factor(ambient_factor),
		_diffuse_factor(diffuse_factor),
		_specular_factor(specular_factor),
		_specular_exponent(specular_exponent) {}

Material::Material(Shader fragment_shader) :
		_type(CUSTOM_SHADER),
		_custom_shader(std::move(fragment_shader)) {}

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