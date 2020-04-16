/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StaticModelGeneratorBox.h"

namespace rheel {

StaticModelGeneratorBox::StaticModelGeneratorBox(vec3 dimensions) :
		_dimensions(dimensions) {}

void StaticModelGeneratorBox::DoGenerate() {
	vec3 position0 = vec3(-0.5f, -0.5f, -0.5f) * _dimensions;
	vec3 position1 = vec3(-0.5f, -0.5f,  0.5f) * _dimensions;
	vec3 position2 = vec3(-0.5f,  0.5f, -0.5f) * _dimensions;
	vec3 position3 = vec3(-0.5f,  0.5f,  0.5f) * _dimensions;
	vec3 position4 = vec3( 0.5f, -0.5f, -0.5f) * _dimensions;
	vec3 position5 = vec3( 0.5f, -0.5f,  0.5f) * _dimensions;
	vec3 position6 = vec3( 0.5f,  0.5f, -0.5f) * _dimensions;
	vec3 position7 = vec3( 0.5f,  0.5f,  0.5f) * _dimensions;

	vec3 normalXn = { -1.0f,  0.0f,  0.0f };
	vec3 normalXp = {  1.0f,  0.0f,  0.0f };
	vec3 normalYn = {  0.0f, -1.0f,  0.0f };
	vec3 normalYp = {  0.0f,  1.0f,  0.0f };
	vec3 normalZn = {  0.0f,  0.0f, -1.0f };
	vec3 normalZp = {  0.0f,  0.0f,  1.0f };

	vertices = {
			{ position0, normalXn, vec2() },
			{ position1, normalXn, vec2() },
			{ position2, normalXn, vec2() },
			{ position3, normalXn, vec2() },

			{ position4, normalXp, vec2() },
			{ position5, normalXp, vec2() },
			{ position6, normalXp, vec2() },
			{ position7, normalXp, vec2() },

			{ position0, normalYn, vec2() },
			{ position4, normalYn, vec2() },
			{ position5, normalYn, vec2() },
			{ position1, normalYn, vec2() },

			{ position2, normalYp, vec2() },
			{ position3, normalYp, vec2() },
			{ position6, normalYp, vec2() },
			{ position7, normalYp, vec2() },

			{ position0, normalZn, vec2() },
			{ position2, normalZn, vec2() },
			{ position6, normalZn, vec2() },
			{ position4, normalZn, vec2() },

			{ position1, normalZp, vec2() },
			{ position5, normalZp, vec2() },
			{ position3, normalZp, vec2() },
			{ position7, normalZp, vec2() }
	};

	indices = {
			 0,  1,  2,  1,  3,  2,
			 4,  7,  5,  4,  6,  7,
			 8,  9, 11,  9, 10, 11,
			12, 13, 15, 15, 14, 12,
			16, 17, 19, 19, 17, 18,
			20, 23, 22, 20, 21, 23
	};
}

}