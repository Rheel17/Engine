/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ENCODING_H
#define RHEELENGINE_ENCODING_H
#include "../../_common.h"

#include <string>

namespace rheel {

class RE_API Encoding {

public:
	/**
	 * Convertes the code point to an UTF-8 encoded string. If c doesn't
	 * represent a valid code point, this will return the empty string.
	 */
	static std::string CodePointToUtf8(char32_t c);

	/**
	 * Convertes the UTF-8 encoded character at the first position of the
	 * character array. Only converts a single character. If the character is
	 * not encoded using UTF-8, this will return -1.
	 */
	static char32_t Utf8ToCodePoint(const char* c);

	/**
	 * Convertes the UTF-8 encoded character at the first position of the
	 * string. Only converts a single character. If the character is not encoded
	 * using UTF-8, this will return -1.
	 */
	static char32_t Utf8ToCodePoint(const std::string& str);

	/**
	 * Converts the UTF-8 encoded character at the first position of the
	 * character array. Only converts a single character. If the character is
	 * not encoded using UTF-8, this will return -1.
	 *
	 * After conversion, this function will advance the pointer forwards to the
	 * next UTF-8 character; shifting it by the size of the UTF-8 character in
	 * bytes.
	 */
	static char32_t ReadUtf8(const char** c);

	/**
	 * Returns the number of bytes needed to encode the unicode code point in
	 * UTF-8. If an invalid code point is given, this will return 0.
	 */
	static size_t Utf8Lenght(char32_t c);

private:
	static char32_t Utf8ToCodePointHelper_(const char* c, size_t* shift);

};

}

#endif
