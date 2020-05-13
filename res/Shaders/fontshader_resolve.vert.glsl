#version 330 core

layout(location = 0) in vec2 vert_Position;

out vec2 vf_Coordinates;

uniform vec4 bounds;

void main(void) {
	gl_Position = vec4(vert_Position * (bounds.zw - bounds.xy) + bounds.xy, 0.0, 1.0);
	vf_Coordinates = gl_Position.xy * 0.5 + 0.5;
}
