/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "_common.h"

namespace rheel {

class RE_API Transform {

public:
	Transform(
			vec3 translation = { 0.0f, 0.0f, 0.0f },
			quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f },
			vec3 scale = { 1.0f, 1.0f, 1.0f });

	/**
	 * Returns the translation component of this transform.
	 */
	const vec3& GetTranslation() const;

	/**
	 * Sets the translation of this transform to the new vector.
	 */
	void SetTranslation(vec3 translation);

	/**
	 * Moves the translation component of this transform with the vector.
	 *
	 * Note: this only updates the internal translation. The move will not take
	 * the rotation or scale into account. Calling this method is equivalent to
	 * SetTranslation(GetTranslation() + vec).
	 */
	void Move(vec3 vec);

	/**
	 * Returns the rotation component of this transform.
	 */
	const quat& GetRotation() const;

	/**
	 * Sets the rotation of this transform to the new quaternion.
	 */
	void SetRotation(quat rotation);

	/**
	 * Rotates the rotational component of this transform with the quaternion.
	 *
	 * Note: this only updates the internal rotation. The rotation will not take
	 * the translation or scale into account. Calling this method is equivalent
	 * to SetRotation(). // TODO left or right multiplication?
	 */
	void Rotate(quat rotation);

	/**
	 * Returns the scale component of this transform.
	 */
	const vec3& GetScale() const;

	/**
	 * Returns the resulting matrix of this transform.
	 */
	const mat4& GetMatrix() const;

private:
	vec3 _translation;
	quat _rotation;
	vec3 _scale;

	mat4 _matrix;
	bool _matrix_dirty;

};

}

#endif
