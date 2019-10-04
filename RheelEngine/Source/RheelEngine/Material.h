/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef MATERIAL_H_
#define MATERIAL_H_
#include "_common.h"

#include "Color.h"
#include "ImageResource.h"

namespace rheel {

class RE_API Material {

public:
	enum MaterialType {
		Colored, Textured
	};

public:
	/**
	 * Creates the default material: a white colored material, with a diffuse
	 * value of 0.9, and a specular value of 0.3
	 */
	Material();

	/**
	 * Creates a colored material with the given color. Additionally, the
	 * diffuse, and specular components can be given. The diffuse and specular
	 * values will be clamped between 0.0 and 1.0. Ambient will be calculated
	 * using 1.0f - diffuse.
	 */
	Material(Color color, float diffuse, float specular, float specularExponent = DEFAULT_SPECULAR_EXPONENT);

	/**
	 * Creates a simple textured material with the given texture. Additionally,
	 * the diffuse, and specular components can be given. The diffuse and
	 * specular values will be clamped between 0.0 and 1.0. Ambient will be
	 * calculated using 1.0f - diffuse.
	 */
	Material(ImageResource& texture, float diffuse, float specular, float specularExponent = DEFAULT_SPECULAR_EXPONENT);

	/**
	 * Creates a fully-textured material, with separate textures for the
	 * ambient, diffuse, and specular components.
	 */
	Material(ImageResource& ambientTexture, ImageResource& diffuseTexture, ImageResource& specularTexture, float specularExponent = DEFAULT_SPECULAR_EXPONENT);

	/**
	 * Creates a fully-textured material, with separate textures for the
	 * ambient, diffuse, and specular components. This constructor also allows
	 * a factor to be set for each of these textures.
	 */
	Material(ImageResource& ambientTexture, ImageResource& diffuseTexture, ImageResource& specularTexture,
			float ambientFactor, float diffuseFactor, float specularFactor,
			float specularExponent = DEFAULT_SPECULAR_EXPONENT);

	/**
	 * Returns the type of this material.
	 */
	MaterialType Type() const;

	/**
	 * Returns a 4-dimensional vector with all the material factors. The
	 * returned vector will be [ Ia, Id, Is, exp ].
	 */
	vec4 MaterialVector() const;

	/**
	 * Returns a const reference to the color of this material. If this is a
	 * textured material, { 0, 0, 0, -1 } is returned.
	 */
	const Color& GetColor() const;

	/**
	 * Returns the ambient light factor
	 */
	float AmbientFactor() const;

	/**
	 * Returns the diffuse light factor
	 */
	float DiffuseFactor() const;

	/**
	 * Returns the specular light factor
	 */
	float SpecularFactor() const;

	/**
	 * Binds the texture images as OpenGL textures.
	 *
	 * 0: ambient
	 * 1: diffuse
	 * 2: specular
	 */
	void BindTextures() const;

	/**
	 * Returns the ambient texture of this material, if present. Otherwise, it
	 * will return nullptr.
	 */
	const ImageResource *AmbientTexture() const;

	/**
	 * Returns the diffuse texture of this material, if present. Otherwise, it
	 * will return nullptr.
	 */
	const ImageResource *DiffuseTexture() const;

	/**
	 * Returns the specular texture of this material, if present. Otherwise, it
	 * will return nullptr.
	 */
	const ImageResource *SpecularTexture() const;

private:
	MaterialType _type;

	Color _color;

	ImageResource *_ambient_texture;
	ImageResource *_diffuse_texture;
	ImageResource *_specular_texture;

	float _ambient_factor;
	float _diffuse_factor;
	float _specular_factor;
	float _specular_exponent;

public:
	static constexpr float DEFAULT_SPECULAR_EXPONENT = 50.0f;

};

}

#endif
