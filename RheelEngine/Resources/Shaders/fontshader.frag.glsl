#version 330 core

in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D fontTexture;

void main(void) {
	vec2 uv = vf_Texture;
	float dist = texture(fontTexture, uv).r;
	frag_Color = vec4(vec3(1.0), dist);

	if (frag_Color.a <= 0) {
		frag_Color = vec4(1.0, 0.0, 0.0, 1.0);
	}
}
