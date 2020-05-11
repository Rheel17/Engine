#version 330 core

layout(location = 0) in vec3 vert_Position;
layout(location = 1) in vec4 inst_Transform;

void main(void) {
	gl_Position = vec4(vert_Position.xy, 0.0, 1.0);
}
