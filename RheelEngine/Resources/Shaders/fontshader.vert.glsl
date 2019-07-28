#version 330 core

layout (location = 0) in vec3 vert_Position;

out vec3 vf_BarycentricCoordinates;

void main(void) {
	gl_Position = vec4(vert_Position.xy, 0.0, 1.0);

	if (vert_Position.z == 0.0) {
		vf_BarycentricCoordinates = vec3(1.0, 0.0, 0.0);
	} else if (vert_Position.z == 1.0) {
		vf_BarycentricCoordinates = vec3(0.0, 1.0, 0.0);
	} else {
		vf_BarycentricCoordinates = vec3(0.0, 0.0, 1.0);
	}
}
