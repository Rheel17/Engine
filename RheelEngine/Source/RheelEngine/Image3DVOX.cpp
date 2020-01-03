/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image3D.h"

namespace rheel {

static bool validateVOX(std::istream& input) {
	// read the signature
	unsigned char header[8];
	input.read((char *) header, 8);

	// check if reading went well
	if (!input.good()) {
		std::cout << "!input.good()" << std::endl;
		return false;
	}

	// compare the header
	return header[0] == 'V' && header[1] == 'O' && header[2] == 'X' && header[3] == ' ' &&
			header[4] == 0x96 && header[5] == 0x00 && header[6] == 0x00 && header[7] == 0x00;
}

static std::pair<unsigned, unsigned> readChunkHeader(std::istream& input, char *outChunk) {
	// read the chunk id
	input.read(outChunk, 4);

	// read the sizes
	unsigned sizes[2];
	input.read((char *) sizes, 8);

	return std::make_pair(sizes[0], sizes[1]);
}

static bool assertChunkName(char *chunk, const char *check, bool force) {
	bool correct = strcmp(chunk, check) == 0;

	if (correct) {
		return true;
	}

	if (force) {
		throw std::runtime_error("An error occurred while reading VOX file: Expected chunk '" +
				std::string(check, 4) + "', got '" + std::string(chunk, 4) + "'");
	}

	return false;
}

void Image3D::_LoadVOX(std::istream& input) {
	if (!validateVOX(input)) {
		throw std::runtime_error("An error occurred while reading VOX file: VOX header not valid.");
	}

	// static storage for chunk names
	char chunk[5] = { 0 };

	// read the main chunk
	readChunkHeader(input, chunk);
	assertChunkName(chunk, "MAIN", true);

	unsigned numModels = 1;

	// read the first chunk
	unsigned firstSize, firstChildren;

	const auto& [firstSize0, firstChildren0] = readChunkHeader(input, chunk);
	firstSize = firstSize0;
	firstChildren = firstChildren0;

	if (strcmp(chunk, "PACK") == 0) {
		// read the PACK chunk
		input.read((char *) &numModels, 4);

		// re-read the first chunk
		const auto& [firstSize1, firstChildren1] = readChunkHeader(input, chunk);
		firstSize = firstSize1;
		firstChildren = firstChildren1;
	}

	// check that we have got a 'size' chunk
	assertChunkName(chunk, "SIZE", true);

	// read the sizes
	unsigned sizes[3];
	input.read((char *) sizes, 12);

	std::cout << sizes[0] << " " << sizes[1] << " " << sizes[2] << std::endl;

	throw std::runtime_error("");

}

}
