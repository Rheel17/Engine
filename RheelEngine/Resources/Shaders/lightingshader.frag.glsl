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

// shadow objects
uniform sampler2DShadow shadowMap;
uniform mat4 lightspaceMatrix;

const int shadowSamples = 16;
const vec2 poissonDisk[16] = vec2[] (
	/* courtesy of http://www.opengl-tutorial.org */
	vec2(-0.94201624, -0.39906216), 
	vec2( 0.94558609, -0.76890725), 
	vec2(-0.09418410, -0.92938870), 
	vec2( 0.34495938,  0.29387760), 
	vec2(-0.91588581,  0.45771432), 
	vec2(-0.81544232, -0.87912464), 
	vec2(-0.38277543,  0.27676845), 
	vec2( 0.97484398,  0.75648379), 
	vec2( 0.44323325, -0.97511554), 
	vec2( 0.53742981, -0.47373420), 
	vec2(-0.26496911, -0.41893023), 
	vec2( 0.79197514,  0.19090188), 
	vec2(-0.24188840,  0.99706507), 
	vec2(-0.81409955,  0.91437590), 
	vec2( 0.19984126,  0.78641367), 
	vec2( 0.14383161, -0.14100790) 
);

// g-buffer parameters
uniform ivec2 gBufferTextureSize;
uniform int sampleCount;
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

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float getShadowFactor(vec3 P, vec3 N, vec3 L) {
	// transform the position to light-space.
	vec4 positionLightspaceClip = lightspaceMatrix * vec4(P, 1.0);
	P = positionLightspaceClip.xyz / positionLightspaceClip.w;
	P = (P + 1.0) / 2.0;
	
	// check for the bounds
	if (P.x < 0 || P.y < 0 || P.x > 1 || P.y > 1) {
		return 1.0;
	}

	// set the bias
	float cosAngle = max(dot(N, L), 0.0);
	float bias = 0.01;
	bias = clamp(bias * tan(acos(cosAngle)), 0.0, 2.0 * bias);

	// sample the shadow map using Poisson PCF
	float shadowFactor = 1.0;
	float sampleFactor = 1.0 / shadowSamples;

	for (int i = 0; i < shadowSamples; i++) {
		int index = int(16.0 * random(floor(P * 1000.0), i)) % 16;
		shadowFactor -= sampleFactor * (1.0 - texture(shadowMap, vec3(P.xy + poissonDisk[i] / 6000.0, P.z), bias));
	}

	return shadowFactor;
}

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
	return abstractLight(material, P, N, -direction, color) * getShadowFactor(P, N, direction);
}

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

	for (int sample = 0; sample < sampleCount; sample++) {
		frag_Color += _main(sample) / sampleCount;
	}
}
