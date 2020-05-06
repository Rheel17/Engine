/*
 * Copyright (c) 2020 Levi van Rheenen
 */

#include <gtest/gtest.h>
#include <RheelEngine/Renderer/Text/Encoding.h>

using namespace rheel;

TEST(Encoding, CodePointToUtf8) {
	EXPECT_EQ(Encoding::CodePointToUtf8(0x00),    std::string(1, 0x00));
	EXPECT_EQ(Encoding::CodePointToUtf8(0x24),    u8"\u0024");
	EXPECT_EQ(Encoding::CodePointToUtf8(0xa2),    u8"\u00a2");
	EXPECT_EQ(Encoding::CodePointToUtf8(0x939),   u8"\u0939");
	EXPECT_EQ(Encoding::CodePointToUtf8(0x20ac),  u8"\u20ac");
	EXPECT_EQ(Encoding::CodePointToUtf8(0xd55c),  u8"\ud55c");
	EXPECT_EQ(Encoding::CodePointToUtf8(0x10348), u8"\U00010348");

	EXPECT_EQ(Encoding::CodePointToUtf8(-1), "");
	EXPECT_EQ(Encoding::CodePointToUtf8(0x110000), "");
	EXPECT_EQ(Encoding::CodePointToUtf8(0x257943), "");
}

TEST(Encoding, Utf8ToCodePoint) {
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\u0000"), 0x00);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\u0024"), 0x24);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\u00a2"), 0xa2);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\u0939"), 0x939);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\u20ac"), 0x20ac);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\ud55c"), 0xd55c);
	EXPECT_EQ(Encoding::Utf8ToCodePoint(u8"\U00010348"), 0x10348);

	EXPECT_EQ(Encoding::Utf8ToCodePoint("\xff"), -1);
	EXPECT_EQ(Encoding::Utf8ToCodePoint("\x8f"), -1);
	EXPECT_EQ(Encoding::Utf8ToCodePoint("\xf1"), -1);
	EXPECT_EQ(Encoding::Utf8ToCodePoint("\xdf\x00"), -1);
	EXPECT_EQ(Encoding::Utf8ToCodePoint("\xef\xbf\x00"), -1);
	EXPECT_EQ(Encoding::Utf8ToCodePoint("\xf0\xbf\xbf\x00"), -1);
}

TEST(Encoding, Utf8Length) {
	EXPECT_EQ(Encoding::Utf8Lenght(0x00),    1);
	EXPECT_EQ(Encoding::Utf8Lenght(0x24),    1);
	EXPECT_EQ(Encoding::Utf8Lenght(0xa2),    2);
	EXPECT_EQ(Encoding::Utf8Lenght(0x939),   3);
	EXPECT_EQ(Encoding::Utf8Lenght(0x20ac),  3);
	EXPECT_EQ(Encoding::Utf8Lenght(0xd55c),  3);
	EXPECT_EQ(Encoding::Utf8Lenght(0x10348), 4);

	EXPECT_EQ(Encoding::Utf8Lenght(-1),       0);
	EXPECT_EQ(Encoding::Utf8Lenght(0x110000), 0);
	EXPECT_EQ(Encoding::Utf8Lenght(0x257943), 0);
}
