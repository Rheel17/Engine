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
	 * Convertes the UTF-8 encoded character at the first position of the string
	 * view. Only converts a single character. If the character is not encoded
	 * using UTF-8 or the string view contains only part of a UTF-8 character,
	 * this will return -1.
	 */
	static char32_t Utf8ToCodePoint(std::string_view sv);

	/**
	 * Converts the UTF-8 encoded character at the first position of the
	 * string view. Only converts a single character. If the character is
	 * not encoded using UTF-8 or the string view contains only part of a UTF-8
	 * character, this will return -1.
	 *
	 * After conversion, this function will advance the string view to the next
	 * UTF-8 character; shifting it by the size of the UTF-8 character in bytes.
	 * Note: this happens only if the resulting character is valid.
	 *
	 * If the string view is empty, it will not be modified and 0 will be
	 * returned.
	 */
	static char32_t ReadUtf8(std::string_view& sv);

	/**
	 * Returns the number of bytes needed to encode the unicode code point in
	 * UTF-8. If an invalid code point is given, this will return 0.
	 */
	static size_t Utf8Lenght(char32_t c);

};

}

#endif
