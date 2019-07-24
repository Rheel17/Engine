#version 330 core

in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D fontTexture;

void main(void) {
	float distance = texture(fontTexture, vf_Texture).r;

	if (distance > 0) {
		frag_Color = vec4(vec3(distance), 1.0);
	} else {
		frag_Color = vec4(0.0);
	}
}
