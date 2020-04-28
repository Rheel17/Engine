/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */

#include <gtest/gtest.h>
#include <RheelEngine/Util/Cache.h>

using namespace rheel;

static const auto& getLength = [](const std::string& str) {
	return str.length();
};

static const auto& getLength2 = [](const std::string& str) {
	return str.length() + 2;
};

TEST(Cache, Keep) {
	Cache<std::string, std::string::size_type, keep_policy> cache;

	EXPECT_EQ(cache.Put("Hello", getLength), true);
	EXPECT_EQ(cache.Put("Hello", getLength2), false);
	EXPECT_EQ(cache.Get("Hello"), 5);
	EXPECT_EQ(cache.ContainsKey("Hello"), true);

	EXPECT_EQ(cache.Get("Test", getLength), 4);
	EXPECT_EQ(cache.Get("Test", getLength2), 4);
	EXPECT_EQ(cache.ContainsKey("Test"), true);

	EXPECT_EQ(cache.Get("Testing", getLength2), 9);
	EXPECT_EQ(cache.ContainsKey("Testing"), true);

	EXPECT_EQ(cache.ContainsKey("Foo"), false);
	EXPECT_EQ(cache.ContainsKey("Bar"), false);

	EXPECT_EQ(cache.GetSize(), 3);
}

TEST(Cache, LeastRecentlyUsed) {
	Cache<std::string, std::string::size_type, least_recently_used_policy> cache(4);

	EXPECT_EQ(cache.GetCapacity(), 4);

	EXPECT_EQ(cache.Put("Hello", getLength), true);
	EXPECT_EQ(cache.Put("Hi", getLength), true);
	EXPECT_EQ(cache.Put("Test", getLength), true);
	EXPECT_EQ(cache.Put("Testing", getLength), true);

	EXPECT_EQ(cache.Get("Hello", getLength2), 5);
	cache.Get("Testing");
	cache.Get("Hello");
	cache.Get("Hi");
	cache.Get("Testing");

	EXPECT_EQ(cache.ContainsKey("Test"), true);
	EXPECT_EQ(cache.Put("Foo", getLength), true);
	EXPECT_EQ(cache.GetSize(), 4);
	EXPECT_EQ(cache.ContainsKey("Test"), false);
	EXPECT_EQ(cache.Put("Test", getLength2), true);
	EXPECT_EQ(cache.Get("Test"), 6);
	EXPECT_EQ(cache.GetSize(), 4);
	EXPECT_EQ(cache.ContainsKey("Hello"), false);
}

TEST(Cache, LastInFirstOut) {
	Cache<std::string, std::string::size_type, last_in_frist_out_policy> cache(4);

	EXPECT_EQ(cache.GetCapacity(), 4);

	EXPECT_EQ(cache.Put("Hello", getLength), true);
	EXPECT_EQ(cache.Put("Hi", getLength), true);
	EXPECT_EQ(cache.Put("Test", getLength), true);
	EXPECT_EQ(cache.Put("Testing", getLength), true);

	EXPECT_EQ(cache.Get("Hello", getLength2), 5);
	cache.Get("Testing");
	cache.Get("Hello");
	cache.Get("Hi");
	cache.Get("Testing");

	EXPECT_EQ(cache.ContainsKey("Hello"), true);
	EXPECT_EQ(cache.Put("Foo", getLength), true);
	EXPECT_EQ(cache.GetSize(), 4);
	EXPECT_EQ(cache.ContainsKey("Hello"), false);
	EXPECT_EQ(cache.Put("Hello", getLength2), true);
	EXPECT_EQ(cache.Get("Hello"), 7);
	EXPECT_EQ(cache.GetSize(), 4);
	EXPECT_EQ(cache.ContainsKey("Hi"), false);
}
