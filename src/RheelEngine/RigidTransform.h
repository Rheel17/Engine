/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RIGIDTRANSFORM_H_
#define RIGIDTRANSFORM_H_
#include "_common.h"

namespace rheel {

struct RE_API TransformOwner {

	virtual ~TransformOwner() = default;
	virtual void TransformChanged() {}

};

class RE_API RigidTransform {

public:
	explicit RigidTransform(
			vec3 translation = { 0.0f, 0.0f, 0.0f },
			quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f });

	explicit RigidTransform(TransformOwner *owner);

	virtual ~RigidTransform() = default;

	RigidTransform(const RigidTransform& t);
	RigidTransform(RigidTransform&& t) noexcept;

	RigidTransform& operator=(const RigidTransform& t);

	RigidTransform& operator=(RigidTransform&& t) noexcept;

	/**
	 * Resets this transform to the identity transform.
	 */
	virtual void SetIdentity();

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
	void Move(const vec3& vec);

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
	 * to SetRotation(rotation * GetRotation()).
	 */
	void Rotate(const quat& rotation);

	/**
	 * Returns the resulting matrix of this transform.
	 */
	const mat4& AsMatrix() const;

	/**
	 * Returns the unit forward vector of this transform, i.e. the
	 * vector (0, 0, -1) as rotated by this transform.
	 */
	vec3 ForwardVector() const;

	/**
	 * Returns the unit up vector of this transform, i.e. the vector (0, 1, 0)
	 * as rotated by this transform
	 */
	vec3 UpVector() const;

	/**
	 * Returns the unit right vector of this transform, i.e. the
	 * vector (1, 0, 0) as rotated by this transform.
	 */
	vec3 RightVector() const;

protected:
	void SetChanged();

	virtual mat4 CalculateMatrix() const;

private:
	vec3 _translation;
	quat _rotation;

	mutable mat4 _matrix{};
	mutable bool _matrix_dirty;

	TransformOwner *_owner = nullptr;

};

}

#endif
