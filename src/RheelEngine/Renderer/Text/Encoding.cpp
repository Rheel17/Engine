/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Encoding.h"

#include <bit>

namespace rheel {

std::string Encoding::CodePointToUtf8(char32_t c) {
	// code point: aaaaa bbbbbbbb cccccccc

	if (c < 0x80) {
		// 0ccccccc
		return std::string(1, static_cast<char>(c));
	}

	auto b_0 = static_cast<uint8_t>(c & 0xffu);
	auto b_1 = static_cast<uint8_t>((c >> 8u) & 0xffu);

	std::string result;

	if (c < 0x800) {
		// 110bbbcc 10cccccc

		result.reserve(2);
		result.push_back(0b11000000u | ((b_1 & 0b00000111u) << 2u) | uint8_t(b_0 >> 6u));
		result.push_back(0b10000000u | (b_0 & 0b00111111u));
	} else if (c < 0x10000) {
		// 1110bbbb 10bbbbcc 10cccccc

		result.reserve(3);
		result.push_back(0b11100000u | uint8_t(b_1 >> 4u));
		result.push_back(0b10000000u | ((b_1 & 0b00001111u) << 2u) | uint8_t(b_0 >> 6u));
		result.push_back(0b10000000u | (b_0 & 0b00111111u));
	} else if (c < 0x110000) {
		// 11110aaa 10aabbbb 10bbbbcc 10cccccc

		auto b_2 = static_cast<uint8_t>((c >> 16u) & 0xffu);

		result.reserve(4);
		result.push_back(0b11110000u | uint8_t(b_2 >> 2u));
		result.push_back(0b10000000u | ((b_2 & 0b00000011u) << 4u) | uint8_t(b_1 >> 4u));
		result.push_back(0b10000000u | ((b_1 & 0b00001111u) << 2u) | uint8_t(b_0 >> 6u));
		result.push_back(0b10000000u | (b_0 & 0b00111111u));
	} else {
		return "";
	}

	return result;
}

char32_t Encoding::Utf8ToCodePoint(std::string_view sv) {
	std::string_view tmp = sv;
	return ReadUtf8(tmp);
}

char32_t Encoding::ReadUtf8(std::string_view& sv) {
	// special case for the empty string_view, since sv[sv.length()] is
	// undefined.
	if (sv.empty()) {
		return 0;
	}

	auto b_0 = static_cast<uint8_t>(sv[0]);
	uint8_t inv = ~b_0;

	// valid 0th-byte binary representations are:
	// 11110'100 -> 00001'011 -> 0B -> 11 [...]
	// 11110'000 -> 00001'111 -> 0F -> 15
	//
	// 1110'1111 -> 0001'0000 -> 10 -> 16 [...]
	// 1110'0000 -> 0001'1111 -> 1F -> 31
	//
	// 110'11111 -> 001'00000 -> 20 -> 32 [...]
	// 110'00000 -> 001'11111 -> 3F -> 63
	//
	// 0'1111111 -> 1'0000000 -> 80 -> 128 [...]
	// 0'0000000 -> 1'1111111 -> FF -> 255
	if (inv < 11 || (64 <= inv && inv < 128)) {
		return -1;
	}

	// count the number of leading zeros to find out what character byte length
	// we have.
	unsigned leading_ones = std::countl_one(b_0);

	// code point: aaaaa bbbbbbbb cccccccc

	if (leading_ones == 0) {
		// 0ccccccc
		sv.remove_prefix(1);
		return b_0;
	}

	if (sv.length() < 2) {
		// incomplete character
		return -1;
	}

	auto b_1 = static_cast<uint8_t>(sv[1]);
	if ((b_1 & 0b11000000u) != 0b10000000) {
		// not a valid continuation byte
		return -1;
	}

	if (leading_ones == 2) {
		// 110bbbcc 10cccccc
		sv.remove_prefix(2);
		return ((b_0 & 0b00011111u) << 6u) | (b_1 & 0b00111111u);
	}

	if (sv.length() < 3) {
		// incomplete character
		return -1;
	}

	auto b_2 = static_cast<uint8_t>(sv[2]);
	if ((b_2 & 0b11000000u) != 0b10000000u) {
		// not a valid continuation byte
		return -1;
	}

	if (leading_ones == 3) {
		// 1110bbbb 10bbbbcc 10cccccc
		sv.remove_prefix(3);
		return ((b_0 & 0b00001111u) << 12u) | ((b_1 & 0b00111111u) << 6u) | (b_2 & 0b00111111u);
	}

	if (sv.length() < 4) {
		// incomplete character
		return -1;
	}

	auto b_3 = static_cast<uint8_t>(sv[3]);
	if ((b_3 & 0b11000000u) != 0b10000000u) {
		// not a valid continuation byte
		return -1;
	}

	if (leading_ones == 4) {
		// 11110aaa 10aabbbb 10bbbbcc 10cccccc
		sv.remove_prefix(4);
		return ((b_0 & 0b00001111u) << 18u) | ((b_1 & 0b00111111u) << 12u) | ((b_2 & 0b00111111u) << 6u) | (b_3 & 0b00111111u);
	}

	return -1;
}

size_t Encoding::Utf8Lenght(char32_t c) {
	if (c < 0x80) {
		return 1;
	}

	if (c < 0x800) {
		return 2;
	} else if (c < 0x10000) {
		return 3;
	} else if (c < 0x110000) {
		return 4;
	}

	return 0;
}

}
