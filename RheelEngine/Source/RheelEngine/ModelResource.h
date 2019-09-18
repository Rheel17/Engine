#ifndef MODELRESOURCE_H_
#define MODELRESOURCE_H_
#include "_common.h"

#include <btBulletDynamicsCommon.h>

#include "Model.h"
#include "Resource.h"

namespace rheel {

class RE_API ModelResource : public Resource<Model> {
	RE_NO_COPY(ModelResource);
	RE_NO_MOVE(ModelResource);

	friend class ResourceManager;
	friend class RigidBodyComponent;

//	static_assert(sizeof(btScalar) == sizeof(float));

private:
//	struct _ConvexHull {
//		std::vector<vec3> points;
//		unsigned point_count;
//		vec3 center;
//	};
//
//	struct _CompoundConvexHull {
//		std::vector<std::unique_ptr<btConvexHullShape>> convex_hull_shapes;
//		std::shared_ptr<btCompoundShape> shape;
//	};

public:
	~ModelResource();

private:
	ModelResource(const std::string& path);
	ModelResource(const std::string& path, Model *model);

//	std::shared_ptr<btCollisionShape> _Shape(const vec3& scale = { 1, 1, 1 }) const;
//	void _CreateConvexHulls() const;
//
//	mutable std::vector<_ConvexHull> _convex_hulls;
//	mutable std::unordered_map<vec3, _CompoundConvexHull> _shapes;

};

}

#endif
