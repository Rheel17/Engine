/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Material.h"

#include "EngineResources.h"
#include "Resources/ResourceManager.h"
#include "Renderer/ImageTexture.h"

namespace rheel {

Material::Material() :
		Material(Color { 1.0f, 1.0f, 1.0f, 1.0f }, 0.9f, 0.3f) {}

Material::Material(Color color, float diffuse, float specular, float specularExponent) :
		_type(Colored), _color(std::move(color)),
		_ambient_texture(nullptr), _diffuse_texture(nullptr),
		_specular_texture(nullptr), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(ImageResource& texture, float diffuse, float specular, float specularExponent) :
		_type(Textured), _color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(&texture), _diffuse_texture(&texture),
		_specular_texture(&texture), _specular_exponent(specularExponent) {

	_diffuse_factor = std::min(1.0f, std::max(0.0f, diffuse));
	_ambient_factor = 1.0f - _diffuse_factor;
	_specular_factor = std::min(1.0f, std::max(0.0f, specular));
}

Material::Material(ImageResource& ambientTexture, ImageResource& diffuseTexture, ImageResource& specularTexture, float specularExponent) :
		Material(ambientTexture, diffuseTexture, specularTexture, 1.0f, 1.0f, 1.0f, specularExponent) {}

Material::Material(ImageResource& ambientTexture, ImageResource& diffuseTexture, ImageResource& specularTexture, float ambientFactor, float diffuseFactor, float specularFactor, float specularExponent) :
		_type(Textured), _color({ 0.0f, 0.0f, 0.0f, -1.0f }),
		_ambient_texture(&ambientTexture), _diffuse_texture(&diffuseTexture),
		_specular_texture(&specularTexture), _ambient_factor(ambientFactor),
		_diffuse_factor(diffuseFactor), _specular_factor(specularFactor),
		_specular_exponent(specularExponent) {}

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
	_ambient_texture->GetImageTexture().Bind(0);
	_diffuse_texture->GetImageTexture().Bind(1);
	_specular_texture->GetImageTexture().Bind(2);
}

const ImageResource *Material::AmbientTexture() const {
	return _ambient_texture;
}

const ImageResource *Material::DiffuseTexture() const {
	return _diffuse_texture;
}

const ImageResource *Material::SpecularTexture() const {
	return _specular_texture;
}

}
