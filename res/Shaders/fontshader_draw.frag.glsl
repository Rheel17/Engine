#version 330 core

in float vf_Color;
in vec4 vf_BarycentricCoordinates;

out float frag_Color;

void main(void) {
	if (vf_BarycentricCoordinates.w != 0.0) {
		float s = vf_BarycentricCoordinates.z;
		float t = vf_BarycentricCoordinates.x;

		if (!((s / 2.0 + t) * (s / 2.0 + t) < t)) {
			discard;
		}
	}

	frag_Color = vf_Color;
}
