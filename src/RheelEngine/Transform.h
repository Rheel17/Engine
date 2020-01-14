/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "_common.h"

#include "RigidTransform.h"

namespace rheel {

class RE_API Transform : public RigidTransform {

public:
	explicit Transform(
			vec3 translation = { 0.0f, 0.0f, 0.0f },
			quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f },
			vec3 scale = { 1.0f, 1.0f, 1.0f });

	explicit Transform(const mat4& matrix);

	explicit Transform(TransformOwner *owner);

	Transform(const Transform& t);

	explicit Transform(const RigidTransform& t);
	explicit Transform(RigidTransform&& t);

	Transform& operator=(const Transform& t);
	Transform& operator=(Transform&& t);

	Transform& operator=(const RigidTransform& t) override;
	Transform& operator=(RigidTransform&& t) override;

	~Transform() override = default;

	/**
	 * Resets this transform to the identity transform.
	 */
	void SetIdentity() override;

	/**
	 * Returns the scale component of this transform.
	 */
	const vec3& GetScale() const;

	/**
	 * Sets the scale component of this transform to the new scale.
	 */
	void SetScale(vec3 scale);

	/**
	 * Scales the scale component of this transform with the given scale vector.
	 *
	 * Note: this only updates the internal scale. The scale will not take the
	 * translation or rotation into account. Calling this method is equivalent
	 * to SetScale(GetScale() * scale).
	 */
	void Scale(const vec3& scale);

protected:
	mat4 CalculateMatrix() const override;

private:
	vec3 _scale;

};

}

#endif
