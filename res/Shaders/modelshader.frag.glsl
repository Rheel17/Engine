/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

out vec4 frag_Color;

// model inputs
in vec3 _vf_Position;
in vec3 _vf_Normal;
in vec2 _vf_Texture;
in vec4 _vf_Material;
in vec4 _vf_Color;

// material parameters
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

#pragma include Shaders_lights_glsl

vec3 calculateColor() {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if (_vf_Color.a < 0) {
		ambient = _vf_Material.x * texture(ambientTexture, _vf_Texture).rgb;
		diffuse = _vf_Material.y * texture(diffuseTexture, _vf_Texture).rgb;
		specular = _vf_Material.z * texture(specularTexture, _vf_Texture).rgb;
	} else {
		vec3 albedo = _vf_Color.rgb;

		ambient = _vf_Material.x * albedo;
		diffuse = _vf_Material.y * albedo;
		specular = _vf_Material.z * albedo;
	}

	vec4 materialParameters = vec4(_vf_Material.w, 0.0, 0.0, 0.0);
	vec3 position = _vf_Position;
	vec3 normal = normalize(_vf_Normal);

	return calculateLights(position, normal, ambient, diffuse, specular, materialParameters);
}

void main(void) {
	frag_Color = vec4(calculateColor(), 1.0);
}
