/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ModelResource.h"

namespace rheel {

std::unordered_map<vec3, std::unique_ptr<ModelResource>> ModelResource::_boxes;

ModelResource& ModelResource::Box(const vec3& dimensions) {
	auto iter = _boxes.find(dimensions);
	if (iter != _boxes.end()) {
		return *iter->second;
	}

	vec3 position_0 = vec3(-1.0f, -1.0f, -1.0f) * dimensions;
	vec3 position_1 = vec3(-1.0f, -1.0f,  1.0f) * dimensions;
	vec3 position_2 = vec3(-1.0f,  1.0f, -1.0f) * dimensions;
	vec3 position_3 = vec3(-1.0f,  1.0f,  1.0f) * dimensions;
	vec3 position_4 = vec3( 1.0f, -1.0f, -1.0f) * dimensions;
	vec3 position_5 = vec3( 1.0f, -1.0f,  1.0f) * dimensions;
	vec3 position_6 = vec3( 1.0f,  1.0f, -1.0f) * dimensions;
	vec3 position_7 = vec3( 1.0f,  1.0f,  1.0f) * dimensions;

	vec3 normal_xn = { -1.0f,  0.0f,  0.0f };
	vec3 normal_xp = {  1.0f,  0.0f,  0.0f };
	vec3 normal_yn = {  0.0f, -1.0f,  0.0f };
	vec3 normal_yp = {  0.0f,  1.0f,  0.0f };
	vec3 normal_zn = {  0.0f,  0.0f, -1.0f };
	vec3 normal_zp = {  0.0f,  0.0f,  1.0f };

	std::vector<Model::Vertex> vertices = {
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

	std::vector<unsigned> indices {
			0,  1,  2,  1,  3,  2,
			4,  7,  5,  4,  6,  7,
			8,  9,  11, 9,  10, 11,
			12, 13, 15, 15, 14, 12,
			16, 17, 19, 19, 17, 18,
			20, 23, 22, 20, 21, 23
	};

	Model *model = new Model(vertices, indices);
	auto resource = std::unique_ptr<ModelResource>(new ModelResource("engine:constructed_box", model));
	ModelResource& ref = *resource;

	_boxes[dimensions] = std::move(resource);
	return ref;
}

}
