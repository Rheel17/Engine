/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "StaticModelGeneratorCapsule.h"

namespace rheel {

StaticModelGeneratorCapsule::StaticModelGeneratorCapsule(float radius, float straightHeight, unsigned subdivisions) :
		_radius(radius),
		_straight_height(straightHeight),
		_subdivisions(subdivisions) {}

void StaticModelGeneratorCapsule::DoGenerate() {
	// add the vertices and indices
	unsigned latDivisions = ((_subdivisions + 1) / 2) * 2 + 1;
	unsigned latSplit = latDivisions / 2;

	for (unsigned latIdx = 0; latIdx <= latDivisions; latIdx++) {
		double latitude = (M_PI * (latIdx - (latIdx > latSplit))) / _subdivisions;

		auto sinLat = static_cast<float>(std::sin(latitude));
		auto cosLat = static_cast<float>(std::cos(latitude));

		vec3 modifier(0.0f, (_straight_height / 2.0f) * static_cast<float>(latIdx <= latSplit ? 1 : -1), 0.0f);

		for (unsigned lonIdx = 0; lonIdx < _subdivisions; lonIdx++) {
			double longitude = (2 * M_PI * lonIdx) / _subdivisions;

			// calculate the vertex
			auto sinLon = static_cast<float>(std::sin(longitude));
			auto cosLon = static_cast<float>(std::cos(longitude));

			vec3 v(sinLon * sinLat, cosLat, cosLon * sinLat);
			vertices.push_back({ v * _radius + modifier, v, vec2() });

			if (latIdx == latDivisions) {
				continue;
			}

			// calculate the indices
			unsigned nextLonIdx = (lonIdx + 1) % _subdivisions;
			unsigned nextLatIdx = (latIdx + 1) % (latDivisions + 1);

			if (latIdx + 1 != latDivisions) {
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