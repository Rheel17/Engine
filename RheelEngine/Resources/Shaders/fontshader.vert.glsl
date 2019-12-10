/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#version 330 core

layout (location = 0) in vec3 vert_Position;

out vec3 vf_BarycentricCoordinates;

uniform int stage;
uniform vec2 multisampleOffset;
uniform vec4 bounds;

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2

void main(void) {
	gl_Position = vec4(vert_Position.xy, 0.0, 1.0);

	if (stage == STAGE_RESOLVE) {
		gl_Position.xy *= bounds.zw - bounds.xy;
		gl_Position.xy += bounds.xy;
	} else {
		gl_Position.xy += multisampleOffset;
	}

	if (vert_Position.z == 0.0) {
		vf_BarycentricCoordinates = vec3(1.0, 0.0, 0.0);
	} else if (vert_Position.z == 1.0) {
		vf_BarycentricCoordinates = vec3(0.0, 1.0, 0.0);
	} else {
		vf_BarycentricCoordinates = vec3(0.0, 0.0, 1.0);
	}
}
