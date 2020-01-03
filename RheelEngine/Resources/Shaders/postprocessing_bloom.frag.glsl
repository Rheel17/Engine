/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#version 330 core

in vec2 vf_Texture;
out vec4 frag_Color;

uniform sampler2D inputTexture;

uniform float kernel[64];
uniform int kernelSize;
uniform float filterStart;
uniform float filterEnd;

uniform int vertical;

vec3 filtered_lookup(vec2 uv) {
	vec3 c = texture(inputTexture, uv).rgb;
	float luminance = 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;

	return c * smoothstep(filterStart, filterEnd, luminance);
}

void main(void) {
	vec2 pixelSize = 1.0 / textureSize(inputTexture, 0);

	if (vertical == 1) {
		pixelSize.x = 0;
	} else {
		pixelSize.y = 0;
	}

	vec4 color = vec4(filtered_lookup(vf_Texture) * kernel[0], 1.0);

	for (int i = 0; i < kernelSize; i++) {
		color.rgb += filtered_lookup(vf_Texture + (i + 1) * pixelSize) * kernel[i + 1];
		color.rgb += filtered_lookup(vf_Texture - (i + 1) * pixelSize) * kernel[i + 1];
	}

	frag_Color = color;
}
