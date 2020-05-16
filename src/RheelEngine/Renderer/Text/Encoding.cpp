/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Encoding.h"

namespace rheel {

std::string Encoding::CodePointToUtf8(char32_t c) {
	if (c < 0) {
		return "";
	}

	// code point: aaaaa bbbbbbbb cccccccc

	if (c < 0x80) {
		// 0ccccccc
		return std::string(1, (char) c);
	}

	uint8_t b0 = static_cast<uint8_t>(c & 0xff);
	uint8_t b1 = static_cast<uint8_t>((c >> 8) & 0xff);

	std::string result;

	if (c < 0x800) {
		// 110bbbcc 10cccccc

		result.reserve(2);
		result.push_back(0b110'00000 | ((b1 & 0b00000111) << 2) | (b0 >> 6));
		result.push_back(0b10'000000 | (b0 & 0b00111111));
	} else if (c < 0x10000) {
		// 1110bbbb 10bbbbcc 10cccccc

		result.reserve(3);
		result.push_back(0b1110'0000 | (b1 >> 4));
		result.push_back(0b10'000000 | ((b1 & 0b00001111) << 2) | (b0 >> 6));
		result.push_back(0b10'000000 | (b0 & 0b00111111));
	} else if (c < 0x110000) {
		// 11110aaa 10aabbbb 10bbbbcc 10cccccc

		uint8_t b2 = static_cast<uint8_t>((c >> 16) & 0xff);

		result.reserve(4);
		result.push_back(0b11110'000 | (b2 >> 2));
		result.push_back(0b10'000000 | ((b2 & 0b00000011) << 4) | (b1 >> 4));
		result.push_back(0b10'000000 | ((b1 & 0b00001111) << 2) | (b0 >> 6));
		result.push_back(0b10'000000 | (b0 & 0b00111111));
	} else {
		return "";
	}

	return result;
}

char32_t Encoding::Utf8ToCodePoint(const char* c) {
	size_t shift;
	return Utf8ToCodePointHelper_(c, &shift);
}

char32_t Encoding::Utf8ToCodePoint(const std::string& str) {
	return Utf8ToCodePoint(str.c_str());
}

char32_t Encoding::ReadUtf8(const char** c) {
	size_t shift = 0;
	char32_t result = Utf8ToCodePointHelper_(*c, &shift);
	*c += shift;
	return result;
}

size_t Encoding::Utf8Lenght(char32_t c) {
	if (c < 0) {
		return 0;
	}

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

char32_t Encoding::Utf8ToCodePointHelper_(const char* c, size_t* shift) {
	uint8_t b0 = static_cast<uint8_t>(c[0]);

	uint8_t inv = ~b0;

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

#if defined(__GNUC__)
	// GCC compiler hack: very fast leading zero count.

	// Count the number of leading zeros of the inverse, i.e. count the number
	// of leading ones on the original. We need to convert back to uint8_t
	// counts because __builtin_clz takes an unsigned int.
	unsigned leadingOnes = __builtin_clz(inv) - 8 * sizeof(unsigned int) + 8;
#else
	unsigned leadingOnes;
	if ((b0 & 0b10000000) == 0) { leadingOnes = 0; }
	else if ((b0 & 0b01000000) == 0) { leadingOnes = 1; }
	else if ((b0 & 0b00100000) == 0) { leadingOnes = 2; }
	else if ((b0 & 0b00010000) == 0) { leadingOnes = 3; }
	else if ((b0 & 0b00001000) == 0) { leadingOnes = 4; }
	// other cases will never happen because they are caught by the inv-checking
	// if statement
#endif

	// code point: aaaaa bbbbbbbb cccccccc

	if (leadingOnes == 0) {
		// 0ccccccc
		*shift = 1;
		return b0;
	}

	uint8_t b1 = static_cast<uint8_t>(c[1]);
	if ((b1 & 0b11'000000) != 0b10'000000) {
		// not a valid continuation byte
		return -1;
	}

	if (leadingOnes == 2) {
		// 110bbbcc 10cccccc
		*shift = 2;
		return ((b0 & 0b00011111) << 6) | (b1 & 0b00111111);
	}

	uint8_t b2 = static_cast<uint8_t>(c[2]);
	if ((b2 & 0b11'000000) != 0b10'000000) {
		// not a valid continuation byte
		return -1;
	}

	if (leadingOnes == 3) {
		// 1110bbbb 10bbbbcc 10cccccc
		*shift = 3;
		return ((b0 & 0b00001111) << 12) | ((b1 & 0b00111111) << 6) | (b2 & 0b00111111);
	}

	uint8_t b3 = static_cast<uint8_t>(c[3]);
	if ((b3 & 0b11'000000) != 0b10'000000) {
		// not a valid continuation byte
		return -1;
	}

	if (leadingOnes == 4) {
		// 11110aaa 10aabbbb 10bbbbcc 10cccccc
		*shift = 4;
		return ((b0 & 0b00001111) << 18) | ((b1 & 0b00111111) << 12) | ((b2 & 0b00111111) << 6) | (b3 & 0b00111111);
	}

	return -1;
}

}