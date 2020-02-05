/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelGenerator.h"

namespace rheel {

Model ModelGenerator::Capsule(float radius, float straightHeight, unsigned subdivisions) {
	std::vector<ModelVertex> vertices;
	std::vector<unsigned> indices;

	// add the vertices and indices
	unsigned latDivisions = ((subdivisions + 1) / 2) * 2 + 1;
	unsigned latSplit = latDivisions / 2;

	for (unsigned latIdx = 0; latIdx <= latDivisions; latIdx++) {
		float latitude = (M_PI * (latIdx - (latIdx > latSplit))) / subdivisions;

		float sinLat = std::sin(latitude);
		float cosLat = std::cos(latitude);

		vec3 modifier(0.0f, (straightHeight / 2.0f) * (latIdx <= latSplit ? 1 : -1), 0.0f);

		for (unsigned lonIdx = 0; lonIdx < subdivisions; lonIdx++) {
			float longitude = (2 * M_PI * lonIdx) / subdivisions;

			// calculate the vertex
			float sinLon = std::sin(longitude);
			float cosLon = std::cos(longitude);

			vec3 v(sinLon * sinLat, cosLat, cosLon * sinLat);
			vertices.push_back({ v * radius + modifier, v, vec2() });

			if (latIdx == latDivisions) {
				continue;
			}

			// calculate the indices
			unsigned nextLonIdx = (lonIdx + 1) % subdivisions;
			unsigned nextLatIdx = (latIdx + 1) % (latDivisions + 1);

			if (latIdx + 1 != latDivisions) {
				indices.push_back(latIdx * subdivisions + lonIdx);
				indices.push_back(nextLatIdx * subdivisions + lonIdx);
				indices.push_back(nextLatIdx * subdivisions + nextLonIdx);
			}

			if (latIdx != 0) {
				indices.push_back(nextLatIdx * subdivisions + nextLonIdx);
				indices.push_back(latIdx * subdivisions + nextLonIdx);
				indices.push_back(latIdx * subdivisions + lonIdx);
			}
		}
	}

	return Model(std::move(vertices), std::move(indices));
}

}
