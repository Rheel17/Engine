/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#version 330 core

in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D inputTexture0;
uniform sampler2D inputTexture1;

uniform float factor0;
uniform float factor1;

void main(void) {
	frag_Color = factor0 * texture(inputTexture0, vf_Texture) + factor1 * texture(inputTexture1, vf_Texture);
}
