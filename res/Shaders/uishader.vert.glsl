/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

layout (location = 0) in vec2 vert_Position;
layout (location = 1) in vec4 vert_Color;
layout (location = 2) in vec2 vert_Texture;

out vec4 vf_Color;
out vec2 vf_Texture;

uniform vec2 screenDimensions;

void main(void) {
	vf_Color = vert_Color;
	vf_Texture = vert_Texture;

	vec2 position = (vert_Position / screenDimensions) * 2 - 1;
	position.y *= -1;

	gl_Position = vec4(position, 0, 1);
}
