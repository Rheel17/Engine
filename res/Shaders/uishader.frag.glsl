/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#version 330 core

#define MODE_COLORED    1
#define MODE_TEXTURED   2

in vec4 vf_Color;
in vec2 vf_Texture;

out vec4 frag_Color;

uniform sampler2D _texture_sampler;
uniform int _ui_mode;
uniform float _alpha;

void main(void) {
	if (_ui_mode == MODE_COLORED) {
		frag_Color = vf_Color;
	} else if (_ui_mode == MODE_TEXTURED){
		frag_Color = texture(_texture_sampler, vf_Texture);
	}

	frag_Color.a *= _alpha;
}
