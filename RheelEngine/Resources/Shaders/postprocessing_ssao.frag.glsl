/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#version 330 core

in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D colorTexture;
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main(void) {
	// frag_Color = vec4(vf_Texture, 1.0, 1.0);
	frag_Color = texture(normalTexture, vf_Texture);
}
