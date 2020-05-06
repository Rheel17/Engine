/*
 * Copyright (c) 2020 Levi van Rheenen
 */

#include <gtest/gtest.h>
#include <RheelEngine/Color.h>

using namespace rheel;

TEST(Color, ConstructFloats) {
	Color c0(0.0f, 0.0f, 0.0f, 0.0f);
	Color c1(1.0f, 1.0f, 1.0f, 1.0f);
	Color c2(0.5f, 0.8f, 0.2f, 0.9f);
	Color c3(0.2f, 0.7f, 0.9f);
	Color c4(-1.0f, 2.1f, -8.1f, 3.0f);

	EXPECT_EQ(static_cast<vec4>(c0), vec4(0.0f, 0.0f, 0.0f, 0.0f));
	EXPECT_EQ(static_cast<vec4>(c1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	EXPECT_EQ(static_cast<vec4>(c2), vec4(0.5f, 0.8f, 0.2f, 0.9f));
	EXPECT_EQ(static_cast<vec4>(c3), vec4(0.2f, 0.7f, 0.9f, 1.0f));
	EXPECT_EQ(static_cast<vec4>(c4), vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

TEST(Color, ConstructInts) {
	Color c0(0, 0, 0, 0);
	Color c1(255, 255, 255, 255);
	Color c2(128, 75, 96, 31);
	Color c3(82, 64, 203);
	Color c4(-100, 256, -1, 9761);

	EXPECT_EQ(static_cast<ivec4>(c0), ivec4(0, 0, 0, 0));
	EXPECT_EQ(static_cast<ivec4>(c1), ivec4(255, 255, 255, 255));
	EXPECT_EQ(static_cast<ivec4>(c2), ivec4(128, 75, 96, 31));
	EXPECT_EQ(static_cast<ivec4>(c3), ivec4(82, 64, 203, 255));
	EXPECT_EQ(static_cast<ivec4>(c4), ivec4(0, 255, 0, 255));
}

TEST(Color, ConstructRGB) {
	Color c0 = 0x000000;
	Color c1 = 0xFFFFFF;
	Color c2 = 0x804b60;
	Color c3(0x5240cb, 42);
	Color c4(0x92d431, -100);

	EXPECT_EQ(static_cast<ivec4>(c0), ivec4(0, 0, 0, 255));
	EXPECT_EQ(static_cast<ivec4>(c1), ivec4(255, 255, 255, 255));
	EXPECT_EQ(static_cast<ivec4>(c2), ivec4(128, 75, 96, 255));
	EXPECT_EQ(static_cast<ivec4>(c3), ivec4(82, 64, 203, 42));
	EXPECT_EQ(static_cast<ivec4>(c4), ivec4(146, 212, 49, 0));
}
