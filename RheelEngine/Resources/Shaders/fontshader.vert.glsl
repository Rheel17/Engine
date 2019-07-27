#version 330 core

layout (location = 0) in vec2 vert_Position;

void main(void) {
	gl_Position = vec4(vert_Position, 0.0, 1.0);
}
