#version 330 core

layout (location = 0) in vec3 vert_Position;

uniform mat4 lightspaceMatrix;

void main(void) {
	gl_Position = lightspaceMatrix * vec4(vert_Position, 1.0);
}
