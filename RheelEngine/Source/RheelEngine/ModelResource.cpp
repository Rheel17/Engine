#include "ModelResource.h"

//#include <VHACD.h>

namespace rheel {

ModelResource::ModelResource(const std::string& path) :
		Resource(path, new Model(path, Model::FormatCollada)) {

	// TODO: automatically find correct model format based on file extension
}

ModelResource::ModelResource(const std::string& path, Model *model) :
		Resource(path, model) {}

ModelResource::~ModelResource() {}

// TODO: this code should create a compound collision shape made up of (several)
// convex mesh shapes that make up the model.
//std::shared_ptr<btCollisionShape> ModelResource::_Shape(const vec3& scale) const {
//	// lookup if we already have a btCollisionShape with this scale
//	auto iter = _shapes.find(scale);
//
//	if (iter != _shapes.end()) {
//		return iter->second.shape;
//	}
//
//	// make sure we have the convex hulls of the shape
//	_CreateConvexHulls();
//
//	// convert the shape into the bullet3 shape.
//	_CompoundConvexHull shape;
//	shape.shape = std::make_shared<btCompoundShape>();
//
//	for (const auto& convexHull : _convex_hulls) {
//		auto convexHullShape = std::make_unique<btConvexHullShape>();
//		convexHullShape->setMargin(0.0f);
//
//		for (const vec3& point : convexHull.points) {
//			convexHullShape->addPoint({ point.x * scale.x, point.y * scale.y, point.z * scale.z }, false);
//		}
//
//		convexHullShape->recalcLocalAabb();
//
//		btTransform transform = btTransform::getIdentity();
//		transform.setOrigin({ -convexHull.center.x * scale.x, -convexHull.center.y * scale.y, -convexHull.center.z * scale.z });
//
//		shape.shape->addChildShape(transform, convexHullShape.get());
//		shape.convex_hull_shapes.push_back(std::move(convexHullShape));
//	}
//
//	// save and return
//	auto result = shape.shape;
//	_shapes[scale] = std::move(shape);
//
//	return result;
//}
//
//void ModelResource::_CreateConvexHulls() const {
//	if (!_convex_hulls.empty()) {
//		return;
//	}
//
//	const Model& model = Get();
//
//	// decompose the mesh into convex hulls
//	const auto& vertices = model.Vertices();
//	float *coordinates = new float[vertices.size() * 3];
//
//	for (unsigned i = 0; i < vertices.size(); i++) {
//		coordinates[3 * i + 0] = vertices[i].position.x;
//		coordinates[3 * i + 1] = vertices[i].position.y;
//		coordinates[3 * i + 2] = vertices[i].position.z;
//	}
//
//	VHACD::IVHACD *vhacd = VHACD::CreateVHACD();
//	bool result = vhacd->Compute(
//			coordinates, vertices.size(),
//			model.Indices().data(), model.Indices().size() / 3,
//			VHACD::IVHACD::Parameters());
//
//	if (!result) {
//		throw std::runtime_error(std::string("V-HACD failed for ") + Name());
//	}
//
//	// combine the convex hulls into a bullet shape
//	auto shape = std::make_unique<btCompoundShape>(true, vhacd->GetNConvexHulls());
//	VHACD::IVHACD::ConvexHull convexHull;
//
//	for (unsigned i = 0; i < vhacd->GetNConvexHulls(); i++) {
//		vhacd->GetConvexHull(i, convexHull);
//		_ConvexHull hull;
//
//		// convert the double array to a btScalar array
//		hull.point_count = convexHull.m_nPoints;
//		hull.points.reserve(convexHull.m_nPoints);
//
//		for (unsigned i = 0; i < convexHull.m_nPoints; i++) {
//			hull.points.emplace_back(
//					convexHull.m_points[3 * i + 0],
//					convexHull.m_points[3 * i + 1],
//					convexHull.m_points[3 * i + 2]);
//		}
//
//		hull.center = {
//				static_cast<float>(convexHull.m_center[0]),
//				static_cast<float>(convexHull.m_center[1]),
//				static_cast<float>(convexHull.m_center[2])
//		};
//
//		std::cout << hull.center << std::endl;
//
//		_convex_hulls.push_back(std::move(hull));
//	}
//
//	vhacd->Clean();
//	vhacd->Release();
//}

}
