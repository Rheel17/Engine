/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#version 330 core

// per-vertex data
layout (location = 0) in vec3 vert_Position;
layout (location = 1) in vec3 vert_Normal;
layout (location = 2) in vec2 vert_Texture;

// per-instance data
layout (location = 3) in mat4 inst_ModelMatrix;
layout (location = 7) in mat4 inst_NormalModelMatrix;
layout (location = 11) in vec4 inst_MaterialVector;
layout (location = 12) in vec4 inst_MaterialColor;
 
uniform mat4 cameraMatrix;

out vec3 vf_Position;
out vec3 vf_Normal;
out vec2 vf_Texture;
out vec4 vf_Material;
out vec4 vf_Color;

void main(void) {
	// calculate the position and normal vector
	vec4 position = inst_ModelMatrix * vec4(vert_Position, 1.0);
	vec4 normal = normalize(inst_NormalModelMatrix * vec4(vert_Normal, 0.0));

	// set variables to be passed to the fragment shader
	vf_Position = position.xyz;
	vf_Normal = normal.xyz;
	vf_Texture = vert_Texture;
	vf_Material = inst_MaterialVector;
	vf_Color = inst_MaterialColor;

	// set the position
	gl_Position = cameraMatrix * position;
}
