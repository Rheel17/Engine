/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#version 330 core

out layout(location = 0) vec4 frag_Color;
out layout(location = 1) vec3 frag_Position;
out layout(location = 2) vec3 frag_Normal;

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

vec3 calculateColor(int sample) {
	vec3 ambient = texelFetch(gBufferAmbient, textureLocation, sample).rgb;

	// no lights: just return the ambient color
	if (lightCount == 0) {
		return ambient;
	} 
	
	// lookup g-buffer
	vec3 position = texelFetch(gBufferPosition, textureLocation, sample).rgb;
	vec3 normal = normalize(texelFetch(gBufferNormal, textureLocation, sample).rgb);
	vec3 diffuse = texelFetch(gBufferDiffuse, textureLocation, sample).rgb;
	vec3 specular = texelFetch(gBufferSpecular, textureLocation, sample).rgb;
	vec4 materialParameters = texelFetch(gBufferMaterialParameters, textureLocation, sample);

	frag_Position = position;
	frag_Normal = normal;

	// perform the calculation
	return calculateLights(position, normal, ambient, diffuse, specular, materialParameters);
}

vec4 _main(int sample) {
	vec4 color = texelFetch(gBufferColor, textureLocation, sample);

	// if the g-buffer color value is an actual color (so alpha >= 0), bypass the lighting
	// system, and just return that color. 
	if (color.a >= 0) {
		return color;
	}

	// apply the lighting shader
	return vec4(calculateColor(sample), 1.0);
}

void main(void) {
	textureLocation = ivec2(vf_Texture.x * gBufferTextureSize.x, vf_Texture.y * gBufferTextureSize.y);

	frag_Position = vec3(0.0);
	frag_Normal = vec3(0.0);

	for (int sample = 0; sample < sampleCount; sample++) {
		frag_Color += _main(sample) / sampleCount;
	}
}
