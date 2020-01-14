/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

layout(location = 0) in vec2 vert_Position;

out vec2 vf_Texture;

void main(void) {
	gl_Position = vec4(vert_Position, 0.0, 1.0);
	vf_Texture = (vert_Position + 1) / 2;
}
