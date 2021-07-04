/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_TRANSFORM_H
#define RHEELENGINE_TRANSFORM_H
#include "_common.h"

namespace rheel {

class RE_API Transform {

public:
	explicit Transform(
			const vec3& translation = { 0.0f, 0.0f, 0.0f },
			const quat& rotation = { 1.0f, 0.0f, 0.0f, 0.0f },
			const vec3& scale = { 1.0f, 1.0f, 1.0f });

	explicit Transform(const mat4& matrix);

	/**
	 * Resets this transform to the identity transform.
	 */
	void SetIdentity();

	/**
	 * Returns the translation component of this transform.
	 */
	const vec3& GetTranslation() const;

	/**
	 * Sets the translation of this transform to the new vector.
	 */
	void SetTranslation(const vec3& translation);

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
	void SetRotation(const quat& rotation);

	/**
	 * Rotates the rotational component of this transform with the quaternion.
	 *
	 * Note: this only updates the internal rotation. The rotation will not take
	 * the translation or scale into account. Calling this method is equivalent
	 * to SetRotation(rotation * GetRotation()).
	 */
	void Rotate(const quat& rotation);

	/**
	 * Sets the 'forward' vector. This will be done by re-calculating the
	 * rotation matrix based on this forward vector.
	 *
	 * \param forward The new forward vector.
	 */
	void SetForward(vec3 forward);

	/**
	 * Returns the scale component of this transform.
	 */
	const vec3& GetScale() const;

	/**
	 * Sets the scale component of this transform to the new scale.
	 */
	void SetScale(const vec3& scale);

	/**
	 * Scales the scale component of this transform with the given scale vector.
	 *
	 * Note: this only updates the internal scale. The scale will not take the
	 * translation or rotation into account. Calling this method is equivalent
	 * to SetScale(GetScale() * scale).
	 */
	void Scale(const vec3& scale);

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

	/**
	 * With A = this, B = t, computes M = AB as matrix multiplication
	 */
	Transform operator*(const Transform& t) const;

	/**
	 * Transforms the vector according to this transform. A vector has a
	 * distance and a direction. So, the translation of this transform has no
	 * effect.
	 * \return The transformed vector.
	 */
	vec3 TransformVector(vec3 v) const;

protected:
	mat4 CalculateMatrix() const;

private:
	vec3 _translation;
	vec3 _scale;
	quat _rotation;

	mutable bool _dirty = false;
	mutable mat4 _matrix;

};

}

#endif
