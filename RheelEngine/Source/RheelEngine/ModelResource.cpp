#include "ModelResource.h"

#include <VHACD.h>

namespace rheel {

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

}
