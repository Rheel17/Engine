// shadow objects
uniform int shadowLevel;
uniform int shadowMapCount;
uniform float baseBias;
uniform sampler2DShadow shadowMap0;
uniform sampler2DShadow shadowMap1;
uniform sampler2DShadow shadowMap2;
uniform sampler2DShadow shadowMap3;
uniform mat4 lightspaceMatrix0;
uniform mat4 lightspaceMatrix1;
uniform mat4 lightspaceMatrix2;
uniform mat4 lightspaceMatrix3;

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

float calculateShadowFactor(vec3 P, vec3 N, vec3 L, sampler2DShadow shadowMap, mat4 lightspaceMatrix, float biasFactor) {
	// transform the position to light-space.
	vec4 positionLightspaceClip = lightspaceMatrix * vec4(P, 1.0);
	vec3 Plight = positionLightspaceClip.xyz / positionLightspaceClip.w;
	Plight = (Plight + 1.0) / 2.0;
	
	// check for the bounds
	if (Plight.x < 0 || Plight.y < 0 || Plight.z < 0 || Plight.x > 1 || Plight.y > 1 || Plight.z > 1) {
		return 2.0;
	}

	// set the bias
	vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
	float cosAngle = max(dot(N, -L), 0.0);
	float bias = (baseBias / biasFactor) * (3 - cosAngle);

	// calculating the PCF parameters
	float pcfLevel = 2.0 * shadowLevel - 1.0;
	float pcfOffset = (pcfLevel - 1.0) / 2.0;
	
	// sample the shadow map using PCF
	float shadowFactor = 1.0;
	float sampleFactor = 1.0 / (pcfLevel * pcfLevel);

	for (float dx = -pcfOffset; dx <= pcfOffset; dx += 1.0) {
		for (float dy = -pcfOffset; dy <= pcfOffset; dy += 1.0) {
			vec2 offset = vec2(dx, dy) * pixelSize;
			shadowFactor -= sampleFactor * (1.0 - texture(shadowMap, vec3(Plight.xy + offset, Plight.z - bias)));
		}
	}

	return shadowFactor;
}

float getShadowFactor(vec3 P, vec3 N, vec3 L) {
	if (shadowLevel <= 0) {
		return 1.0;
	}

	float shadowFactor;
	
	shadowFactor = calculateShadowFactor(P, N, L, shadowMap0, lightspaceMatrix0, 8.0);
	if (shadowFactor <= 1.0) { return shadowFactor; }
	if (shadowMapCount <= 1) { return 1.0; }

	shadowFactor = calculateShadowFactor(P, N, L, shadowMap1, lightspaceMatrix1, 4.0);
	if (shadowFactor <= 1.0) { return shadowFactor; }
	if (shadowMapCount <= 2) { return 1.0; }

	shadowFactor = calculateShadowFactor(P, N, L, shadowMap2, lightspaceMatrix2, 2.0);
	if (shadowFactor <= 1.0) { return shadowFactor; }
	if (shadowMapCount <= 3) { return 1.0; }

	shadowFactor = calculateShadowFactor(P, N, L, shadowMap3, lightspaceMatrix3, 1.0);
	if (shadowFactor <= 1.0) { return shadowFactor; }
	if (shadowMapCount <= 4) { return 1.0; }

	return 1.0;
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
	vec3 light = abstractLight(material, P, N, -direction, color);

	if (light.r <= 0 || light.g <= 0 || light.b <= 0) {
		return light;
	}

	return light * getShadowFactor(P, N, direction);
}

vec3 calculateLights(vec3 position, vec3 normal, vec3 ambient, vec3 diffuse, vec3 specular, vec4 materialParameters) {
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
