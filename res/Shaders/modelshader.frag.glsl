/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

out vec4 frag_Color;

// model inputs
in vec3 vf_Position;
in vec3 vf_Normal;
in vec2 vf_Texture;
in vec4 vf_Material;
in vec4 vf_Color;

// material parameters
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

#pragma include Shaders_lights_glsl

vec3 calculateColor() {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	if (vf_Color.a < 0.0) {
		ambient = vf_Material.x * texture(ambientTexture, vf_Texture).rgb;
		diffuse = vf_Material.y * texture(diffuseTexture, vf_Texture).rgb;
		specular = vf_Material.z * texture(specularTexture, vf_Texture).rgb;
	} else {
		vec3 albedo = vf_Color.rgb;

		ambient = vf_Material.x * albedo;
		diffuse = vf_Material.y * albedo;
		specular = vf_Material.z * albedo;
	}

	vec4 materialParameters = vec4(vf_Material.w, 0.0, 0.0, 0.0);
	vec3 position = vf_Position;
	vec3 normal = normalize(vf_Normal);

	return calculateLights(position, normal, ambient, diffuse, specular, materialParameters);
}

void main(void) {
	frag_Color = vec4(calculateColor(), 1.0);
}
