/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#version 330 core

#define MODE_COLORED    1
#define MODE_TEXTURED   2

in vec4 vf_Color;
in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D textureSampler;
uniform int uiMode;
uniform float alpha;

void main(void) {
	if (uiMode == MODE_COLORED) {
		frag_Color = vf_Color;
	} else if (uiMode == MODE_TEXTURED){
		frag_Color = texture(textureSampler, vf_Texture);
	}

	frag_Color.a *= alpha;
}
