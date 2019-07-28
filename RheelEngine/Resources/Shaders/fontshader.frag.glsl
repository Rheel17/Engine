#version 330 core

out vec4 frag_Color;

uniform int stage;

#define STAGE_TRIANGLES		0
#define STAGE_BEZIER		1
#define STAGE_RESOLVE		2

void handleTriangles(void) {
	frag_Color = vec4(1.0);
}

void handleBezier(void) {
	frag_Color = vec4(1.0);
}

void handleResolve(void) {
	frag_Color = vec4(1.0);
}

void main(void) {
	switch (stage) {
		case STAGE_TRIANGLES:	handleTriangles();	break;
		case STAGE_BEZIER:		handleBezier();		break;
		case STAGE_RESOLVE:		handleResolve();	break;
	}
}
