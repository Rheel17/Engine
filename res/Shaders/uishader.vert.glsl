/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

layout (location = 0) in vec2 vert_Position;
layout (location = 1) in vec4 vert_Color;
layout (location = 2) in vec2 vert_Texture;

out vec4 vf_Color;
out vec2 vf_Texture;
out vec2 vf_Coordinates;

uniform vec2 _screen_dimensions;

void main(void) {
	vf_Color = vert_Color;
	vf_Texture = vert_Texture;

	vec2 position = (vert_Position / _screen_dimensions) * 2.0 - 1.0;
	position.y *= -1.0;

	gl_Position = vec4(position, 0.0, 1.0);
	vf_Coordinates = position;
}
