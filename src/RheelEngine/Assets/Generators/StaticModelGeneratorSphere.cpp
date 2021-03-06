/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StaticModelGeneratorSphere.h"

namespace rheel {

StaticModelGeneratorSphere::StaticModelGeneratorSphere(float radius, unsigned int subdivisions) :
		_radius(radius),
		_subdivisions(subdivisions) {}

void StaticModelGeneratorSphere::DoGenerate() {
	// add the vertices and indices
	for (unsigned latIdx = 0; latIdx <= _subdivisions; latIdx++) {
		double latitude = (M_PI * latIdx) / _subdivisions;

		auto sinLat = static_cast<float>(std::sin(latitude));
		auto cosLat = static_cast<float>(std::cos(latitude));

		for (unsigned lonIdx = 0; lonIdx < _subdivisions; lonIdx++) {
			double longitude = (2 * M_PI * lonIdx) / _subdivisions;

			// calculate the vertex
			auto sinLon = static_cast<float>(std::sin(longitude));
			auto cosLon = static_cast<float>(std::cos(longitude));

			vec3 v(sinLon * sinLat, cosLat, cosLon * sinLat);
			vertices.push_back({ v * _radius, v, vec2() });

			if (latIdx == _subdivisions) {
				continue;
			}

			// calculate the indices
			unsigned nextLonIdx = (lonIdx + 1) % _subdivisions;
			unsigned nextLatIdx = (latIdx + 1) % (_subdivisions + 1);

			if (latIdx + 1 != _subdivisions) {
				indices.push_back(latIdx * _subdivisions + lonIdx);
				indices.push_back(nextLatIdx * _subdivisions + lonIdx);
				indices.push_back(nextLatIdx * _subdivisions + nextLonIdx);
			}

			if (latIdx != 0) {
				indices.push_back(nextLatIdx * _subdivisions + nextLonIdx);
				indices.push_back(latIdx * _subdivisions + nextLonIdx);
				indices.push_back(latIdx * _subdivisions + lonIdx);
			}
		}
	}
}

}