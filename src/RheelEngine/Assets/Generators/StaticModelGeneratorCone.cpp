/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "StaticModelGeneratorCone.h"

namespace rheel {

StaticModelGeneratorCone::StaticModelGeneratorCone(float radius, float height, unsigned int subdivisions) :
		_radius(radius),
		_height(height),
		_subdivisions(subdivisions) {}

void StaticModelGeneratorCone::DoGenerate() {
	// add the cone part
	// float hypotenuse = std::sqrt((_radius * _radius) + (_height * _height));
	float cone_angle = std::atan2(_radius, _height);
	float sin_cone = std::sin(cone_angle);
	float cos_cone = std::cos(cone_angle);

	for (unsigned int i = 0; i < _subdivisions * 2; i++) {
		float angle = static_cast<float>(2.0f * M_PI * i) / static_cast<float>(2 * _subdivisions);
		float cos_a = std::cos(angle);
		float sin_a = std::sin(angle);

		vec3 n{ cos_a * cos_cone, sin_cone, sin_a * cos_cone };
		vec3 v;

		if ((i % 2) == 0) {
			v = { _radius * cos_a, -_height / 2.0f, _radius * sin_a };
		} else {
			// the top part of the cone is added multiple times, because the normal
			// has to be different every time.
			v = { 0.0f, _height / 2.0f, 0.0f };
		}

		vertices.push_back(model_vertex{ v, n });
	}

	for (unsigned int i = 0; i < _subdivisions; i++) {
		unsigned int v = 2 * i;
		unsigned int v_prime = (2 * i + 2) % (_subdivisions * 2);
		unsigned int h = 2 * i + 1;

		indices.push_back(v);
		indices.push_back(h);
		indices.push_back(v_prime);
	}

	// close the cone
	for (unsigned int i = 0; i < _subdivisions; i++) {
		float angle = static_cast<float>(2.0f * M_PI * i) / static_cast<float>(_subdivisions);
		float cos_a = std::cos(angle);
		float sin_a = std::sin(angle);

		vertices.push_back(model_vertex{
				{ _radius * cos_a, -_height / 2.0f, _radius * sin_a },
				{ 0.0f,            -1.0f,           0.0f }
		});
	}

	for (unsigned int i = 2; i < _subdivisions; i++) {
		indices.push_back(2 * _subdivisions + i);
		indices.push_back(2 * _subdivisions);
		indices.push_back(2 * _subdivisions + i - 1);
	}
}

}
