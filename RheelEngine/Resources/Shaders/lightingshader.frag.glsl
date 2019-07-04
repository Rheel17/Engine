#version 400 core

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
ivec2 textureLocation;

// light parameters
uniform int lights_type[64];
uniform vec3 lights_position[64];
uniform vec3 lights_direction[64];
uniform vec4 lights_color[64];
uniform float lights_attenuation[64];
uniform float lights_spot_attenuation[64];

uniform int lightCount;

// light type definitions
#define TYPE_PointLight         0
#define TYPE_SpotLight          1
#define TYPE_DirectionalLight   2

// camera parameters
uniform vec3 cameraPosition;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularExponent;
};

vec3 abstractLight(Material material, vec3 P, vec3 N, vec3 L, vec4 color) {
	// calculate vectors
	vec3 R = reflect(L, N);
	vec3 V = normalize(cameraPosition - P);

    //diffuse component
	vec3 Id = max(dot(N, L), 0.0) * material.diffuse;

	// specular component
	float s = max(0.0, dot(R, V));
    if (s > 0) {
        s = pow(s, material.specularExponent);
    }

	vec3 Is = s * material.specular;

	// combine the components with the light color
	return (Id + Is) * color.rgb * color.a;
}

vec3 pointLight(Material material, vec3 P, vec3 N, vec3 position, vec4 color, float attenuation) {
	// calculate the to-light vector, and store the distance.
	vec3 L = position - P;
	float dist = length(L);
    L /= dist;

	// calculate light and apply attenuation factor
	return abstractLight(material, P, N, L, color) / (1 + attenuation * dist * dist);
}

vec3 spotLight(Material material, vec3 P, vec3 N, vec3 position, vec3 direction, vec4 color, float attenuation, float spotAttenuation) {
	// calculate the to-light vector, and store the distance.
	vec3 L = position - P;
	float dist = length(L);
    L /= dist;

	// calculate cosine of spot angle
	float f = dot(L, -direction);

	if (f <= 0.0) {
		// if the cosine of spot angle is <= 0, no light falls on this point, so don't continue.
		// return vec3(1, 0, 1);
	}

	// apply spot attenuation
	f = pow(f, spotAttenuation);

	// calculate light, and apply both attenuations factors
	return abstractLight(material, P, N, L, color) * f / (1 + attenuation * dist * dist);
}

vec3 directionalLight(Material material, vec3 P, vec3 N, vec3 direction, vec4 color) {
	return abstractLight(material, P, N, -direction, color);
}

vec3 _main(void) {
	vec3 ambient = texelFetch(gBufferAmbient, textureLocation, gl_SampleID).rgb;

	// no lights: just return the ambient color
	if (lightCount == 0) {
		return ambient;
	} 
	
	// lookup g-buffer
	vec3 position = texelFetch(gBufferPosition, textureLocation, gl_SampleID).rgb;
	vec3 normal = normalize(texelFetch(gBufferNormal, textureLocation, gl_SampleID).rgb);
	vec3 diffuse = texelFetch(gBufferDiffuse, textureLocation, gl_SampleID).rgb;
	vec3 specular = texelFetch(gBufferSpecular, textureLocation, gl_SampleID).rgb;
	vec4 materialParameters = texelFetch(gBufferMaterialParameters, textureLocation, gl_SampleID);

	// build material
	Material material;
	material.ambient = ambient;
	material.diffuse = diffuse;
	material.specular = specular;
	material.specularExponent = materialParameters.x;

	// color accumulator
	vec3 color = vec3(0.0);

	// for each light: calculate color of pixel with that light, and add it to the accumulator.
	for (int i = 0; i < lightCount; i++) {
		switch (lights_type[i]) {
			case TYPE_PointLight:
				color += pointLight(material, position, normal, lights_position[i], lights_color[i], lights_attenuation[i]);
				break;
			case TYPE_SpotLight:
				color += spotLight(material, position, normal, lights_position[i], lights_direction[i], lights_color[i], lights_attenuation[i], lights_spot_attenuation[i]);
				break;
			case TYPE_DirectionalLight:
				color += directionalLight(material, position, normal, lights_direction[i], lights_color[i]);
				break;
		}
	}

	// return accumulated color, combined with the ambient color.
	return ambient + color;
}

void main(void) {
	textureLocation = ivec2(vf_Texture.x * gBufferTextureSize.x, vf_Texture.y * gBufferTextureSize.y);
	vec4 color = texelFetch(gBufferColor, textureLocation, gl_SampleID);

	// if the g-buffer color value is an actual color (so alpha >= 0), bypass the lighting
	// system, and just return that color. 
	if (color.a >= 0) {
		frag_Color = color;
		return;
	}

	// apply the lighting shader
	frag_Color = vec4(_main(), 1.0);
}
