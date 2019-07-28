#version 330 core

in vec3 vf_BarycentricCoordinates;

out vec4 frag_Color;

uniform int stage;
uniform vec4 color;

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2

void handleTriangles(void) {
	frag_Color = vec4(1.0);
}

void handleBezier(void) {
	float s = vf_BarycentricCoordinates.z;
	float t = vf_BarycentricCoordinates.x;

	if ((s / 2.0 + t) * (s / 2.0 + t) < t) {
	 	frag_Color = vec4(1.0);
	} else {
		discard;
	}
}

void handleResolve(void) {
	frag_Color = color;
}

void main(void) {
	switch (stage) {
		case STAGE_TRIANGLES:	handleTriangles();	break;
		case STAGE_BEZIER:		handleBezier();		break;
		case STAGE_RESOLVE:		handleResolve();	break;
	}
}
