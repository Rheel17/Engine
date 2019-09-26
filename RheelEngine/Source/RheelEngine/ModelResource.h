/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
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

public:
	struct ConvexHull {
		std::vector<vec3> points;
		unsigned point_count;
		vec3 center;
	};

public:
	~ModelResource();

	const std::vector<ConvexHull>& ConvexHulls() const;

private:
	ModelResource(const std::string& path);
	ModelResource(const std::string& path, Model *model);

	mutable std::vector<ConvexHull> _convex_hulls;

};

}

#endif
