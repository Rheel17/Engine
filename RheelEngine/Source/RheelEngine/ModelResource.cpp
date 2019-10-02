/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "ModelResource.h"

#include <VHACD.h>

namespace rheel {

std::unordered_map<vec3, std::unique_ptr<ModelResource>> ModelResource::_boxes;
std::unordered_map<float, std::unique_ptr<ModelResource>> ModelResource::_spheres;

ModelResource::ModelResource(const std::string& path) :
		Resource(path, new Model(path, Model::FormatCollada)) {

	// TODO: automatically find correct model format based on file extension
}

ModelResource::ModelResource(const std::string& path, Model *model) :
		Resource(path, model) {}

ModelResource::~ModelResource() {}

const std::vector<ModelResource::ConvexHull>& ModelResource::ConvexHulls() const {
	if (_convex_hulls.empty()) {
		const Model& model = Get();

		// decompose the mesh into convex hulls
		const auto& vertices = model.Vertices();
		float *coordinates = new float[vertices.size() * 3];

		for (unsigned i = 0; i < vertices.size(); i++) {
			coordinates[3 * i + 0] = vertices[i].position.x;
			coordinates[3 * i + 1] = vertices[i].position.y;
			coordinates[3 * i + 2] = vertices[i].position.z;
		}

		VHACD::IVHACD *vhacd = VHACD::CreateVHACD();
		bool result = vhacd->Compute(
				coordinates, vertices.size(),
				model.Indices().data(), model.Indices().size() / 3,
				VHACD::IVHACD::Parameters());

		if (!result) {
			throw std::runtime_error(std::string("V-HACD failed for ") + Name());
		}

		// combine the convex hulls into a bullet shape
		auto shape = std::make_unique<btCompoundShape>(true, vhacd->GetNConvexHulls());
		VHACD::IVHACD::ConvexHull convexHull;

		for (unsigned i = 0; i < vhacd->GetNConvexHulls(); i++) {
			vhacd->GetConvexHull(i, convexHull);
			ConvexHull hull;

			// convert the double array to a btScalar array
			hull.point_count = convexHull.m_nPoints;
			hull.points.reserve(convexHull.m_nPoints);

			for (unsigned i = 0; i < convexHull.m_nPoints; i++) {
				hull.points.emplace_back(
						convexHull.m_points[3 * i + 0],
						convexHull.m_points[3 * i + 1],
						convexHull.m_points[3 * i + 2]);
			}

			hull.center = {
					static_cast<float>(convexHull.m_center[0]),
					static_cast<float>(convexHull.m_center[1]),
					static_cast<float>(convexHull.m_center[2])
			};

			_convex_hulls.push_back(std::move(hull));
		}

		vhacd->Clean();
		vhacd->Release();
	}

	return _convex_hulls;
}

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
