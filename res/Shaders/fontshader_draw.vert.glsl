#version 330 core

layout(location = 0) in vec3 vert_Position;
layout(location = 1) in vec4 inst_Transform;

out float vf_Color;
out vec4 vf_BarycentricCoordinates;

uniform vec2 bufferSize;

const vec2 subpixelTransforms[4] = vec2[](
	vec2(-0.25,  0.75),
	vec2( 0.75,  0.25),
	vec2(-0.75, -0.25),
	vec2( 0.25, -0.75)
);

const vec4 barycentricCoordinates[4] = vec4[](
	vec4(1.0, 0.0, 0.0, 1.0),
	vec4(0.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 0.0)
);

void main(void) {
	gl_Position = vec4(vert_Position.xy * inst_Transform.zw + inst_Transform.xy, 0.0, 1.0);
	gl_Position.xy += subpixelTransforms[gl_InstanceID] / bufferSize;

	vf_Color = float(1 << gl_InstanceID) / 256.0;
	vf_BarycentricCoordinates = barycentricCoordinates[int(vert_Position.z)];
}