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

};

}

#endif
