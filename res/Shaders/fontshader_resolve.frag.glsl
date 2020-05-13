#version 330 core

in vec2 vf_Coordinates;
out vec4 frag_Color;

uniform sampler2D drawTexture;
uniform vec4 color;

void main(void) {
	float value = texture(drawTexture, vf_Coordinates).r;
	int data = int(value * 256.0);

	int samples = (data & 1) + ((data & 2) >> 1) + ((data & 4) >> 2) + ((data & 8) >> 3);

	if (samples == 0) {
		discard;
	}

	float fraction = float(samples) / 4.0;
	frag_Color = color * fraction;
}
