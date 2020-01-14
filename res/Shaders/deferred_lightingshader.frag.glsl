/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

out vec4 frag_Color;

in vec2 vf_Texture;

// g-buffer textures
uniform sampler2DMS gBufferColor;
uniform sampler2DMS gBufferPosition;
uniform sampler2DMS gBufferNormal;
uniform sampler2DMS gBufferAmbient;
uniform sampler2DMS gBufferDiffuse;
uniform sampler2DMS gBufferSpecular;
uniform sampler2DMS gBufferMaterialParameters;

// g-buffer parameters
uniform ivec2 gBufferTextureSize;
uniform int sampleCount;
ivec2 textureLocation;

#pragma include Shaders_lights_glsl

vec3 calculateColor(int sampleIndex) {
	vec3 ambient = texelFetch(gBufferAmbient, textureLocation, sampleIndex).rgb;

	// no lights: just return the ambient color
	if (lightCount == 0) {
		return ambient;
	} 
	
	// lookup g-buffer
	vec3 position = texelFetch(gBufferPosition, textureLocation, sampleIndex).rgb;
	vec3 normal = normalize(texelFetch(gBufferNormal, textureLocation, sampleIndex).rgb);
	vec3 diffuse = texelFetch(gBufferDiffuse, textureLocation, sampleIndex).rgb;
	vec3 specular = texelFetch(gBufferSpecular, textureLocation, sampleIndex).rgb;
	vec4 materialParameters = texelFetch(gBufferMaterialParameters, textureLocation, sampleIndex);

	// perform the calculation
	return calculateLights(position, normal, ambient, diffuse, specular, materialParameters);
}

vec4 _main(int sampleIndex) {
	vec4 color = texelFetch(gBufferColor, textureLocation, sampleIndex);

	// if the g-buffer color value is an actual color (so alpha >= 0), bypass the lighting
	// system, and just return that color. 
	if (color.a >= 0) {
		return color;
	}

	// apply the lighting shader
	return vec4(calculateColor(sampleIndex), 1.0);
}

void main(void) {
	textureLocation = ivec2(vf_Texture.x * gBufferTextureSize.x, vf_Texture.y * gBufferTextureSize.y);

	for (int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
		frag_Color += _main(sampleIndex) / sampleCount;
	}
}
