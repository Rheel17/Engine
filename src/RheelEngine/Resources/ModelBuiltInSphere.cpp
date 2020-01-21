/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ModelResource.h"

namespace rheel {

std::unordered_map<vec2, std::unique_ptr<ModelResource>> ModelResource::_spheres;

ModelResource& ModelResource::Sphere(float radius, unsigned subdivisions) {
	auto iter = _spheres.find({ radius, subdivisions });
	if (iter != _spheres.end()) {
		return *iter->second;
	}

	std::vector<Model::Vertex> vertices;
	std::vector<unsigned> indices;

	// add the vertices and indices
	for (unsigned latIdx = 0; latIdx <= subdivisions; latIdx++) {
		float latitude = (M_PI * latIdx) / subdivisions;

		float sinLat = std::sin(latitude);
		float cosLat = std::cos(latitude);

		for (unsigned lonIdx = 0; lonIdx < subdivisions; lonIdx++) {
			float longitude = (2 * M_PI * lonIdx) / subdivisions;

			// calculate the vertex
			float sinLon = std::sin(longitude);
			float cosLon = std::cos(longitude);

			vec3 v(sinLon * sinLat, cosLat, cosLon * sinLat);
			vertices.push_back({ v * radius, v, vec2() });

			if (latIdx == subdivisions) {
				continue;
			}

			// calculate the indices
			unsigned nextLonIdx = (lonIdx + 1) % subdivisions;
			unsigned nextLatIdx = (latIdx + 1) % (subdivisions + 1);

			if (latIdx + 1 != subdivisions) {
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

	Model *model = new Model(vertices, indices);
	auto resource = std::unique_ptr<ModelResource>(new ModelResource("engine:constructed_sphere", model));
	ModelResource& ref = *resource;

	_spheres[{ radius, subdivisions }] = std::move(resource);
	return ref;
}

}
