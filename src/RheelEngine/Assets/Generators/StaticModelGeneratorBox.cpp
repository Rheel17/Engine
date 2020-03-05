/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StaticModelGeneratorBox.h"

namespace rheel {

StaticModelGeneratorBox::StaticModelGeneratorBox(vec3 dimensions) :
		_dimensions(dimensions) {}

void StaticModelGeneratorBox::DoGenerate() {
	vec3 position_0 = vec3(-0.5f, -0.5f, -0.5f) * _dimensions;
	vec3 position_1 = vec3(-0.5f, -0.5f,  0.5f) * _dimensions;
	vec3 position_2 = vec3(-0.5f,  0.5f, -0.5f) * _dimensions;
	vec3 position_3 = vec3(-0.5f,  0.5f,  0.5f) * _dimensions;
	vec3 position_4 = vec3( 0.5f, -0.5f, -0.5f) * _dimensions;
	vec3 position_5 = vec3( 0.5f, -0.5f,  0.5f) * _dimensions;
	vec3 position_6 = vec3( 0.5f,  0.5f, -0.5f) * _dimensions;
	vec3 position_7 = vec3( 0.5f,  0.5f,  0.5f) * _dimensions;

	vec3 normal_xn = { -1.0f,  0.0f,  0.0f };
	vec3 normal_xp = {  1.0f,  0.0f,  0.0f };
	vec3 normal_yn = {  0.0f, -1.0f,  0.0f };
	vec3 normal_yp = {  0.0f,  1.0f,  0.0f };
	vec3 normal_zn = {  0.0f,  0.0f, -1.0f };
	vec3 normal_zp = {  0.0f,  0.0f,  1.0f };

	vertices = {
			{ position_0, normal_xn, vec2() },
			{ position_1, normal_xn, vec2() },
			{ position_2, normal_xn, vec2() },
			{ position_3, normal_xn, vec2() },

			{ position_4, normal_xp, vec2() },
			{ position_5, normal_xp, vec2() },
			{ position_6, normal_xp, vec2() },
			{ position_7, normal_xp, vec2() },

			{ position_0, normal_yn, vec2() },
			{ position_4, normal_yn, vec2() },
			{ position_5, normal_yn, vec2() },
			{ position_1, normal_yn, vec2() },

			{ position_2, normal_yp, vec2() },
			{ position_3, normal_yp, vec2() },
			{ position_6, normal_yp, vec2() },
			{ position_7, normal_yp, vec2() },

			{ position_0, normal_zn, vec2() },
			{ position_2, normal_zn, vec2() },
			{ position_6, normal_zn, vec2() },
			{ position_4, normal_zn, vec2() },

			{ position_1, normal_zp, vec2() },
			{ position_5, normal_zp, vec2() },
			{ position_3, normal_zp, vec2() },
			{ position_7, normal_zp, vec2() }
	};

	indices = {
			0,  1,  2,  1,  3,  2,
			4,  7,  5,  4,  6,  7,
			8,  9,  11, 9,  10, 11,
			12, 13, 15, 15, 14, 12,
			16, 17, 19, 19, 17, 18,
			20, 23, 22, 20, 21, 23
	};
}

}