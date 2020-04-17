/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */

#include <gtest/gtest.h>
#include <RheelEngine/Animator/SplineInterpolator.h>

static constexpr float e = 0.0001f;

using namespace rheel;

TEST(SplineInterpolator, InterpolatingPoints) {
	// example from https://timodenk.com/blog/cubic-spline-interpolation/

	SplineInterpolator<float> interpolator;
	interpolator.AddPoint(0, 21);
	interpolator.AddPoint(1, 24);
	interpolator.AddPoint(2, 24);
	interpolator.AddPoint(3, 18);
	interpolator.AddPoint(4, 16);

	float v0 = interpolator(0.0f);
	float v1 = interpolator(1.0f);
	float v2 = interpolator(2.0f);
	float v3 = interpolator(3.0f);
	float v4 = interpolator(4.0f);

	EXPECT_NEAR(v0, 21.0f, e);
	EXPECT_NEAR(v1, 24.0f, e);
	EXPECT_NEAR(v2, 24.0f, e);
	EXPECT_NEAR(v3, 18.0f, e);
	EXPECT_NEAR(v4, 16.0f, e);
}
