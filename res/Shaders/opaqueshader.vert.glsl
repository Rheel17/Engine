/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

// per-vertex data
layout (location = 0) in vec3 vert_Position;

// per-instance data
layout (location = 3) in mat4 inst_ModelMatrix;
 
uniform mat4 lightspaceMatrix;

void main(void) {
	// calculate the position
	vec4 position = inst_ModelMatrix * vec4(vert_Position, 1.0);

	// set the position
	gl_Position = lightspaceMatrix * position;
}
