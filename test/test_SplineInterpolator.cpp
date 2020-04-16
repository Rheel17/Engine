/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */

#include <gtest/gtest.h>
#include <RheelEngine/Animator/SplineInterpolator.h>

static constexpr float e = 0.000001f;

using namespace rheel;

TEST(SplineInterpolator, Matrix) {
	// https://timodenk.com/blog/cubic-spline-interpolation/

	SplineInterpolator<float> interpolator;
	interpolator.AddPoint(0, 21);
	interpolator.AddPoint(1, 24);
	interpolator.AddPoint(2, 24);
	interpolator.AddPoint(3, 18);
	interpolator.AddPoint(4, 16);

	interpolator(0.5f);
}
