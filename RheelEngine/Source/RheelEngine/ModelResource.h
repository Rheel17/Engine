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

public:
	/**
	 * Returns a built-in box model with the dimensions given. The life-time of
	 * this model will be the entire program duration.
	 */
	static ModelResource& Box(const vec3& halfDimensions);

	/**
	 * Returns a built-in sphere model with the radius given. The life-time of
	 * this model will be the entire program duration.
	 */
	static ModelResource& Sphere(float radius, unsigned subdivisions = 18);

	/**
	 * Returns a built-in capsule model with the radius and straight height
	 * given. The total height of the capsule is 2*radius + straightHeight. The
	 * life-time of this model will be the entire program duration.
	 */
	static ModelResource& Capsule(float radius, float straightHeight, unsigned subdivisions = 18);

private:
	static std::unordered_map<vec3, std::unique_ptr<ModelResource>> _boxes;
	static std::unordered_map<vec2, std::unique_ptr<ModelResource>> _spheres;
	static std::unordered_map<vec3, std::unique_ptr<ModelResource>> _capsules;

};

}

#endif
