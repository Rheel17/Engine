/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PreparedText.h"

namespace rheel {

void PreparedText::Prepare(const prepare_text_input& input, gl::Buffer& transformBuffer, gl::DrawElementsIndirectBuffer& indirectBuffer) {
	unsigned spaceWidth = input.font.CharacterWidth(U' ');
	unsigned tabWidth = spaceWidth * input.tab_width;
	unsigned lineHeight = input.font_size * input.line_height;

	size_t currentOffset = 0;
	size_t lastWordBoundary = 0;

	unsigned currentLineWidth = 0;

	const char32_t* text = input.text;
	char32_t c;

	while ((c = *(text++))) {
		if (c == U'\n') {
			// handle newline
			continue;
		}

		if (c == U' ') {
			// handle space
			continue;
		}

		if (c == U'\t') {
			// handle tab
			continue;
		}

		// handle normal character
	}
}

}