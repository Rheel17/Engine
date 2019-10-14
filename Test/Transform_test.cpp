/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */

#include <gtest/gtest.h>
#include <RheelEngine/Transform.h>

using namespace rheel;

static constexpr float e = 0.000001f;

TEST(Transform, InitialComponents) {
	Transform t;

	const auto& translation = t.GetTranslation();
	EXPECT_EQ(vec3(0, 0, 0), translation);

	const auto& rotation = t.GetRotation();
	EXPECT_EQ(quat(1, 0, 0, 0), rotation);

	const auto& scale = t.GetScale();
	EXPECT_EQ(vec3(1, 1, 1), scale);
}

TEST(Transform, InitialVectors) {
	Transform t;

	auto f = t.ForwardVector();
	EXPECT_EQ(vec3(0, 0, -1), f);

	auto u = t.UpVector();
	EXPECT_EQ(vec3(0, 1, 0), u);

	auto r = t.RightVector();
	EXPECT_EQ(vec3(1, 0, 0), r);
}

TEST(Transform, SetTranslation) {
	Transform t;

	t.SetTranslation(vec3(-8, 1, 2));
	EXPECT_EQ(vec3(-8, 1, 2), t.GetTranslation());

	t.SetTranslation(vec3(3, -5, 1));
	EXPECT_EQ(vec3(3, -5, 1), t.GetTranslation());
}

TEST(Transform, Move) {
	Transform t;

	t.Move(vec3(-8, 1, 2));
	EXPECT_EQ(vec3(-8, 1, 2), t.GetTranslation());

	t.Move(vec3(3, -5, 1));
	EXPECT_EQ(vec3(-5, -4, 3), t.GetTranslation());
}

TEST(Transform, SetRotation) {
	Transform t;

	t.SetRotation(quat(0.704416f, 0.5416752f, 0.4545195f, -0.0616284f));
	EXPECT_EQ(quat(0.704416f, 0.5416752f, 0.4545195f, -0.0616284f), t.GetRotation());

	t.SetRotation(vec3(M_PI / 2.0f, 0.0f, 0.0f));
	const auto& rotation = t.GetRotation();
	EXPECT_NEAR(0.7071068f, rotation.w, e);
	EXPECT_NEAR(0.7071068f, rotation.x, e);
	EXPECT_NEAR(0.0f, rotation.y, e);
	EXPECT_NEAR(0.0f, rotation.z, e);
}

TEST(Transform, Rotate) {
	Transform t;

	// [ 0.5, -0.2, 0.1 ]
	t.Rotate(quat(0.9641015f, 0.2410258f, -0.1089122f, 0.0235152f));
	EXPECT_EQ(quat(0.9641015f, 0.2410258f, -0.1089122f, 0.0235152f), t.GetRotation());

	// [ 1.2, -0.9, -0.4 ]
	t.Rotate(quat(0.6795639f, 0.5696167f, -0.250827f, -0.3883497f));
	const auto& rotation = t.GetRotation();
	EXPECT_NEAR(rotation.w,  0.4996903f, e);
	EXPECT_NEAR(rotation.x,  0.6647665f, e);
	EXPECT_NEAR(rotation.y, -0.4228325f, e);
	EXPECT_NEAR(rotation.z, -0.3600108f, e);
}

TEST(Transform, SetScale) {
	Transform t;

	t.SetScale(vec3(0.6f, 0.2f, 0.9f));
	EXPECT_EQ(vec3(0.6f, 0.2f, 0.9f), t.GetScale());

	t.SetScale(vec3(1.6f, 0.1f, 5.6f));
	EXPECT_EQ(vec3(1.6f, 0.1f, 5.6f), t.GetScale());
}

TEST(Transform, Scale) {
	Transform t;

	t.Scale(vec3(3.0f, 5.0f, 1.3f));
	EXPECT_EQ(vec3(3.0f, 5.0f, 1.3f), t.GetScale());

	t.Scale(vec3(0.4f, 2.0f, 0.6f));
	EXPECT_EQ(vec3(3.0f * 0.4f, 5.0f * 2.0f, 1.3f * 0.6f), t.GetScale());
}

TEST(Transform, AsMatrix_Identity) {
	Transform t;

	const mat4& matrix = t.AsMatrix();
	EXPECT_EQ(1.0f, matrix[0][0]);
	EXPECT_EQ(0.0f, matrix[0][1]);
	EXPECT_EQ(0.0f, matrix[0][2]);
	EXPECT_EQ(0.0f, matrix[0][3]);
	EXPECT_EQ(0.0f, matrix[1][0]);
	EXPECT_EQ(1.0f, matrix[1][1]);
	EXPECT_EQ(0.0f, matrix[1][2]);
	EXPECT_EQ(0.0f, matrix[1][3]);
	EXPECT_EQ(0.0f, matrix[2][0]);
	EXPECT_EQ(0.0f, matrix[2][1]);
	EXPECT_EQ(1.0f, matrix[2][2]);
	EXPECT_EQ(0.0f, matrix[2][3]);
	EXPECT_EQ(0.0f, matrix[3][0]);
	EXPECT_EQ(0.0f, matrix[3][1]);
	EXPECT_EQ(0.0f, matrix[3][2]);
	EXPECT_EQ(1.0f, matrix[3][3]);
}

TEST(Transform, AsMatrix_Translate) {
	Transform t;
	t.SetTranslation(vec3(2.0f, -1.0f, 5.0f));

	const mat4& matrix = t.AsMatrix();
	EXPECT_EQ( 1.0f, matrix[0][0]);
	EXPECT_EQ( 0.0f, matrix[0][1]);
	EXPECT_EQ( 0.0f, matrix[0][2]);
	EXPECT_EQ( 0.0f, matrix[0][3]);
	EXPECT_EQ( 0.0f, matrix[1][0]);
	EXPECT_EQ( 1.0f, matrix[1][1]);
	EXPECT_EQ( 0.0f, matrix[1][2]);
	EXPECT_EQ( 0.0f, matrix[1][3]);
	EXPECT_EQ( 0.0f, matrix[2][0]);
	EXPECT_EQ( 0.0f, matrix[2][1]);
	EXPECT_EQ( 1.0f, matrix[2][2]);
	EXPECT_EQ( 0.0f, matrix[2][3]);
	EXPECT_EQ( 2.0f, matrix[3][0]);
	EXPECT_EQ(-1.0f, matrix[3][1]);
	EXPECT_EQ( 5.0f, matrix[3][2]);
	EXPECT_EQ( 1.0f, matrix[3][3]);
}

TEST(Transform, AsMatrix_Rotate) {
	// euler: [ -1.0, 0.5, -0.2 ]
	Transform t;
	t.SetRotation(quat(0.8342113f, -0.4838763f, 0.1696580f, -0.2029076));

	const mat4& matrix = t.AsMatrix();
	EXPECT_NEAR( 0.86008934f, matrix[0][0], e);
	EXPECT_NEAR(-0.50272258f, matrix[0][1], e);
	EXPECT_NEAR(-0.08669680f, matrix[0][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[0][3]);
	EXPECT_NEAR( 0.17434874f, matrix[1][0], e);
	EXPECT_NEAR( 0.44938452f, matrix[1][1], e);
	EXPECT_NEAR(-0.87615984f, matrix[1][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[1][3]);
	EXPECT_NEAR( 0.47942554f, matrix[2][0], e);
	EXPECT_NEAR( 0.73846026f, matrix[2][1], e);
	EXPECT_NEAR( 0.47415988f, matrix[2][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[2][3]);
	EXPECT_EQ  ( 0.0f,        matrix[3][0]);
	EXPECT_EQ  ( 0.0f,        matrix[3][1]);
	EXPECT_EQ  ( 0.0f,        matrix[3][2]);
	EXPECT_EQ  ( 1.0f,        matrix[3][3]);
}

TEST(Transform, AsMatrix_Scale) {
	Transform t;
	t.SetScale(vec3(0.5f, 2.3f, 4.2f));

	const mat4& matrix = t.AsMatrix();
	EXPECT_EQ( 0.5f, matrix[0][0]);
	EXPECT_EQ( 0.0f, matrix[0][1]);
	EXPECT_EQ( 0.0f, matrix[0][2]);
	EXPECT_EQ( 0.0f, matrix[0][3]);
	EXPECT_EQ( 0.0f, matrix[1][0]);
	EXPECT_EQ( 2.3f, matrix[1][1]);
	EXPECT_EQ( 0.0f, matrix[1][2]);
	EXPECT_EQ( 0.0f, matrix[1][3]);
	EXPECT_EQ( 0.0f, matrix[2][0]);
	EXPECT_EQ( 0.0f, matrix[2][1]);
	EXPECT_EQ( 4.2f, matrix[2][2]);
	EXPECT_EQ( 0.0f, matrix[2][3]);
	EXPECT_EQ( 0.0f, matrix[3][0]);
	EXPECT_EQ( 0.0f, matrix[3][1]);
	EXPECT_EQ( 0.0f, matrix[3][2]);
	EXPECT_EQ( 1.0f, matrix[3][3]);
}

TEST(Transform, AsMatrix) {
	// euler: [0.2, -1.1, -0.6]
	Transform t;
	t.SetTranslation(vec3(2.0f, -5.0f, 7.0f));
	t.SetRotation(quat(0.7949582f, 0.2350021f, -0.4716957f, -0.3005306f));
	t.SetScale(vec3(1.5f, 3.0f, 0.2f));

	const mat4& matrix = t.AsMatrix();
	EXPECT_NEAR( 0.5615536f, matrix[0][0], e);
	EXPECT_NEAR(-1.0492762f, matrix[0][1], e);
	EXPECT_NEAR( 0.9130591f, matrix[0][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[0][3]);
	EXPECT_NEAR( 0.7683589f, matrix[1][0], e);
	EXPECT_NEAR( 2.1267323, matrix[1][1], e);
	EXPECT_NEAR( 1.9714549, matrix[1][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[1][3]);
	EXPECT_NEAR(-0.1782415, matrix[2][0], e);
	EXPECT_NEAR(-0.0180231, matrix[2][1], e);
	EXPECT_NEAR( 0.0889109, matrix[2][2], e);
	EXPECT_EQ  ( 0.0f,        matrix[2][3]);
	EXPECT_EQ  ( 2.0f,        matrix[3][0]);
	EXPECT_EQ  (-5.0f,        matrix[3][1]);
	EXPECT_EQ  ( 7.0f,        matrix[3][2]);
	EXPECT_EQ  ( 1.0f,        matrix[3][3]);
}
