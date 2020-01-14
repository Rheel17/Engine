/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image3D.h"

namespace rheel {

static constexpr std::array<Color, 256> default_palette = { {
		{ 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.8f, 1.0f, 1.0f }, { 1.0f, 0.6f, 1.0f, 1.0f },
		{ 1.0f, 0.4f, 1.0f, 1.0f }, { 1.0f, 0.2f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.8f, 1.0f },
		{ 1.0f, 0.8f, 0.8f, 1.0f }, { 1.0f, 0.6f, 0.8f, 1.0f }, { 1.0f, 0.4f, 0.8f, 1.0f }, { 1.0f, 0.2f, 0.8f, 1.0f },
		{ 1.0f, 0.0f, 0.8f, 1.0f }, { 1.0f, 1.0f, 0.6f, 1.0f }, { 1.0f, 0.8f, 0.6f, 1.0f }, { 1.0f, 0.6f, 0.6f, 1.0f },
		{ 1.0f, 0.4f, 0.6f, 1.0f }, { 1.0f, 0.2f, 0.6f, 1.0f }, { 1.0f, 0.0f, 0.6f, 1.0f }, { 1.0f, 1.0f, 0.4f, 1.0f },
		{ 1.0f, 0.8f, 0.4f, 1.0f }, { 1.0f, 0.6f, 0.4f, 1.0f }, { 1.0f, 0.4f, 0.4f, 1.0f }, { 1.0f, 0.2f, 0.4f, 1.0f },
		{ 1.0f, 0.0f, 0.4f, 1.0f }, { 1.0f, 1.0f, 0.2f, 1.0f }, { 1.0f, 0.8f, 0.2f, 1.0f }, { 1.0f, 0.6f, 0.2f, 1.0f },
		{ 1.0f, 0.4f, 0.2f, 1.0f }, { 1.0f, 0.2f, 0.2f, 1.0f }, { 1.0f, 0.0f, 0.2f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.8f, 0.0f, 1.0f }, { 1.0f, 0.6f, 0.0f, 1.0f }, { 1.0f, 0.4f, 0.0f, 1.0f }, { 1.0f, 0.2f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 0.8f }, { 1.0f, 0.8f, 1.0f, 0.8f }, { 1.0f, 0.6f, 1.0f, 0.8f },
		{ 1.0f, 0.4f, 1.0f, 0.8f }, { 1.0f, 0.2f, 1.0f, 0.8f }, { 1.0f, 0.0f, 1.0f, 0.8f }, { 1.0f, 1.0f, 0.8f, 0.8f },
		{ 1.0f, 0.8f, 0.8f, 0.8f }, { 1.0f, 0.6f, 0.8f, 0.8f }, { 1.0f, 0.4f, 0.8f, 0.8f }, { 1.0f, 0.2f, 0.8f, 0.8f },
		{ 1.0f, 0.0f, 0.8f, 0.8f }, { 1.0f, 1.0f, 0.6f, 0.8f }, { 1.0f, 0.8f, 0.6f, 0.8f }, { 1.0f, 0.6f, 0.6f, 0.8f },
		{ 1.0f, 0.4f, 0.6f, 0.8f }, { 1.0f, 0.2f, 0.6f, 0.8f }, { 1.0f, 0.0f, 0.6f, 0.8f }, { 1.0f, 1.0f, 0.4f, 0.8f },
		{ 1.0f, 0.8f, 0.4f, 0.8f }, { 1.0f, 0.6f, 0.4f, 0.8f }, { 1.0f, 0.4f, 0.4f, 0.8f }, { 1.0f, 0.2f, 0.4f, 0.8f },
		{ 1.0f, 0.0f, 0.4f, 0.8f }, { 1.0f, 1.0f, 0.2f, 0.8f }, { 1.0f, 0.8f, 0.2f, 0.8f }, { 1.0f, 0.6f, 0.2f, 0.8f },
		{ 1.0f, 0.4f, 0.2f, 0.8f }, { 1.0f, 0.2f, 0.2f, 0.8f }, { 1.0f, 0.0f, 0.2f, 0.8f }, { 1.0f, 1.0f, 0.0f, 0.8f },
		{ 1.0f, 0.8f, 0.0f, 0.8f }, { 1.0f, 0.6f, 0.0f, 0.8f }, { 1.0f, 0.4f, 0.0f, 0.8f }, { 1.0f, 0.2f, 0.0f, 0.8f },
		{ 1.0f, 0.0f, 0.0f, 0.8f }, { 1.0f, 1.0f, 1.0f, 0.6f }, { 1.0f, 0.8f, 1.0f, 0.6f }, { 1.0f, 0.6f, 1.0f, 0.6f },
		{ 1.0f, 0.4f, 1.0f, 0.6f }, { 1.0f, 0.2f, 1.0f, 0.6f }, { 1.0f, 0.0f, 1.0f, 0.6f }, { 1.0f, 1.0f, 0.8f, 0.6f },
		{ 1.0f, 0.8f, 0.8f, 0.6f }, { 1.0f, 0.6f, 0.8f, 0.6f }, { 1.0f, 0.4f, 0.8f, 0.6f }, { 1.0f, 0.2f, 0.8f, 0.6f },
		{ 1.0f, 0.0f, 0.8f, 0.6f }, { 1.0f, 1.0f, 0.6f, 0.6f }, { 1.0f, 0.8f, 0.6f, 0.6f }, { 1.0f, 0.6f, 0.6f, 0.6f },
		{ 1.0f, 0.4f, 0.6f, 0.6f }, { 1.0f, 0.2f, 0.6f, 0.6f }, { 1.0f, 0.0f, 0.6f, 0.6f }, { 1.0f, 1.0f, 0.4f, 0.6f },
		{ 1.0f, 0.8f, 0.4f, 0.6f }, { 1.0f, 0.6f, 0.4f, 0.6f }, { 1.0f, 0.4f, 0.4f, 0.6f }, { 1.0f, 0.2f, 0.4f, 0.6f },
		{ 1.0f, 0.0f, 0.4f, 0.6f }, { 1.0f, 1.0f, 0.2f, 0.6f }, { 1.0f, 0.8f, 0.2f, 0.6f }, { 1.0f, 0.6f, 0.2f, 0.6f },
		{ 1.0f, 0.4f, 0.2f, 0.6f }, { 1.0f, 0.2f, 0.2f, 0.6f }, { 1.0f, 0.0f, 0.2f, 0.6f }, { 1.0f, 1.0f, 0.0f, 0.6f },
		{ 1.0f, 0.8f, 0.0f, 0.6f }, { 1.0f, 0.6f, 0.0f, 0.6f }, { 1.0f, 0.4f, 0.0f, 0.6f }, { 1.0f, 0.2f, 0.0f, 0.6f },
		{ 1.0f, 0.0f, 0.0f, 0.6f }, { 1.0f, 1.0f, 1.0f, 0.4f }, { 1.0f, 0.8f, 1.0f, 0.4f }, { 1.0f, 0.6f, 1.0f, 0.4f },
		{ 1.0f, 0.4f, 1.0f, 0.4f }, { 1.0f, 0.2f, 1.0f, 0.4f }, { 1.0f, 0.0f, 1.0f, 0.4f }, { 1.0f, 1.0f, 0.8f, 0.4f },
		{ 1.0f, 0.8f, 0.8f, 0.4f }, { 1.0f, 0.6f, 0.8f, 0.4f }, { 1.0f, 0.4f, 0.8f, 0.4f }, { 1.0f, 0.2f, 0.8f, 0.4f },
		{ 1.0f, 0.0f, 0.8f, 0.4f }, { 1.0f, 1.0f, 0.6f, 0.4f }, { 1.0f, 0.8f, 0.6f, 0.4f }, { 1.0f, 0.6f, 0.6f, 0.4f },
		{ 1.0f, 0.4f, 0.6f, 0.4f }, { 1.0f, 0.2f, 0.6f, 0.4f }, { 1.0f, 0.0f, 0.6f, 0.4f }, { 1.0f, 1.0f, 0.4f, 0.4f },
		{ 1.0f, 0.8f, 0.4f, 0.4f }, { 1.0f, 0.6f, 0.4f, 0.4f }, { 1.0f, 0.4f, 0.4f, 0.4f }, { 1.0f, 0.2f, 0.4f, 0.4f },
		{ 1.0f, 0.0f, 0.4f, 0.4f }, { 1.0f, 1.0f, 0.2f, 0.4f }, { 1.0f, 0.8f, 0.2f, 0.4f }, { 1.0f, 0.6f, 0.2f, 0.4f },
		{ 1.0f, 0.4f, 0.2f, 0.4f }, { 1.0f, 0.2f, 0.2f, 0.4f }, { 1.0f, 0.0f, 0.2f, 0.4f }, { 1.0f, 1.0f, 0.0f, 0.4f },
		{ 1.0f, 0.8f, 0.0f, 0.4f }, { 1.0f, 0.6f, 0.0f, 0.4f }, { 1.0f, 0.4f, 0.0f, 0.4f }, { 1.0f, 0.2f, 0.0f, 0.4f },
		{ 1.0f, 0.0f, 0.0f, 0.4f }, { 1.0f, 1.0f, 1.0f, 0.2f }, { 1.0f, 0.8f, 1.0f, 0.2f }, { 1.0f, 0.6f, 1.0f, 0.2f },
		{ 1.0f, 0.4f, 1.0f, 0.2f }, { 1.0f, 0.2f, 1.0f, 0.2f }, { 1.0f, 0.0f, 1.0f, 0.2f }, { 1.0f, 1.0f, 0.8f, 0.2f },
		{ 1.0f, 0.8f, 0.8f, 0.2f }, { 1.0f, 0.6f, 0.8f, 0.2f }, { 1.0f, 0.4f, 0.8f, 0.2f }, { 1.0f, 0.2f, 0.8f, 0.2f },
		{ 1.0f, 0.0f, 0.8f, 0.2f }, { 1.0f, 1.0f, 0.6f, 0.2f }, { 1.0f, 0.8f, 0.6f, 0.2f }, { 1.0f, 0.6f, 0.6f, 0.2f },
		{ 1.0f, 0.4f, 0.6f, 0.2f }, { 1.0f, 0.2f, 0.6f, 0.2f }, { 1.0f, 0.0f, 0.6f, 0.2f }, { 1.0f, 1.0f, 0.4f, 0.2f },
		{ 1.0f, 0.8f, 0.4f, 0.2f }, { 1.0f, 0.6f, 0.4f, 0.2f }, { 1.0f, 0.4f, 0.4f, 0.2f }, { 1.0f, 0.2f, 0.4f, 0.2f },
		{ 1.0f, 0.0f, 0.4f, 0.2f }, { 1.0f, 1.0f, 0.2f, 0.2f }, { 1.0f, 0.8f, 0.2f, 0.2f }, { 1.0f, 0.6f, 0.2f, 0.2f },
		{ 1.0f, 0.4f, 0.2f, 0.2f }, { 1.0f, 0.2f, 0.2f, 0.2f }, { 1.0f, 0.0f, 0.2f, 0.2f }, { 1.0f, 1.0f, 0.0f, 0.2f },
		{ 1.0f, 0.8f, 0.0f, 0.2f }, { 1.0f, 0.6f, 0.0f, 0.2f }, { 1.0f, 0.4f, 0.0f, 0.2f }, { 1.0f, 0.2f, 0.0f, 0.2f },
		{ 1.0f, 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 0.8f, 1.0f, 0.0f }, { 1.0f, 0.6f, 1.0f, 0.0f },
		{ 1.0f, 0.4f, 1.0f, 0.0f }, { 1.0f, 0.2f, 1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.8f, 0.0f },
		{ 1.0f, 0.8f, 0.8f, 0.0f }, { 1.0f, 0.6f, 0.8f, 0.0f }, { 1.0f, 0.4f, 0.8f, 0.0f }, { 1.0f, 0.2f, 0.8f, 0.0f },
		{ 1.0f, 0.0f, 0.8f, 0.0f }, { 1.0f, 1.0f, 0.6f, 0.0f }, { 1.0f, 0.8f, 0.6f, 0.0f }, { 1.0f, 0.6f, 0.6f, 0.0f },
		{ 1.0f, 0.4f, 0.6f, 0.0f }, { 1.0f, 0.2f, 0.6f, 0.0f }, { 1.0f, 0.0f, 0.6f, 0.0f }, { 1.0f, 1.0f, 0.4f, 0.0f },
		{ 1.0f, 0.8f, 0.4f, 0.0f }, { 1.0f, 0.6f, 0.4f, 0.0f }, { 1.0f, 0.4f, 0.4f, 0.0f }, { 1.0f, 0.2f, 0.4f, 0.0f },
		{ 1.0f, 0.0f, 0.4f, 0.0f }, { 1.0f, 1.0f, 0.2f, 0.0f }, { 1.0f, 0.8f, 0.2f, 0.0f }, { 1.0f, 0.6f, 0.2f, 0.0f },
		{ 1.0f, 0.4f, 0.2f, 0.0f }, { 1.0f, 0.2f, 0.2f, 0.0f }, { 1.0f, 0.0f, 0.2f, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.8f, 0.0f, 0.0f }, { 1.0f, 0.6f, 0.0f, 0.0f }, { 1.0f, 0.4f, 0.0f, 0.0f }, { 1.0f, 0.2f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f, 0.933333f }, { 1.0f, 0.0f, 0.0f, 0.866667f }, { 1.0f, 0.0f, 0.0f, 0.733333f },
		{ 1.0f, 0.0f, 0.0f, 0.666667f }, { 1.0f, 0.0f, 0.0f, 0.533333f }, { 1.0f, 0.0f, 0.0f, 0.466667f },
		{ 1.0f, 0.0f, 0.0f, 0.333333f }, { 1.0f, 0.0f, 0.0f, 0.266667f }, { 1.0f, 0.0f, 0.0f, 0.133333f },
		{ 1.0f, 0.0f, 0.0f, 0.0666667f }, { 1.0f, 0.0f, 0.933333f, 0.0f }, { 1.0f, 0.0f, 0.866667f, 0.0f },
		{ 1.0f, 0.0f, 0.733333f, 0.0f }, { 1.0f, 0.0f, 0.666667f, 0.0f }, { 1.0f, 0.0f, 0.533333f, 0.0f },
		{ 1.0f, 0.0f, 0.466667f, 0.0f }, { 1.0f, 0.0f, 0.333333f, 0.0f }, { 1.0f, 0.0f, 0.266667f, 0.0f },
		{ 1.0f, 0.0f, 0.133333f, 0.0f }, { 1.0f, 0.0f, 0.0666667f, 0.0f }, { 1.0f, 0.933333f, 0.0f, 0.0f },
		{ 1.0f, 0.866667f, 0.0f, 0.0f }, { 1.0f, 0.733333f, 0.0f, 0.0f }, { 1.0f, 0.666667f, 0.0f, 0.0f },
		{ 1.0f, 0.533333f, 0.0f, 0.0f }, { 1.0f, 0.466667f, 0.0f, 0.0f }, { 1.0f, 0.333333f, 0.0f, 0.0f },
		{ 1.0f, 0.266667f, 0.0f, 0.0f }, { 1.0f, 0.133333f, 0.0f, 0.0f }, { 1.0f, 0.0666667f, 0.0f, 0.0f },
		{ 1.0f, 0.933333f, 0.933333f, 0.933333f }, { 1.0f, 0.866667f, 0.866667f, 0.866667f },
		{ 1.0f, 0.733333f, 0.733333f, 0.733333f }, { 1.0f, 0.666667f, 0.666667f, 0.666667f },
		{ 1.0f, 0.533333f, 0.533333f, 0.533333f }, { 1.0f, 0.466667f, 0.466667f, 0.466667f },
		{ 1.0f, 0.333333f, 0.333333f, 0.333333f }, { 1.0f, 0.266667f, 0.266667f, 0.266667f },
		{ 1.0f, 0.133333f, 0.133333f, 0.133333f }, { 1.0f, 0.0666667f, 0.0666667f, 0.0666667f }
} };

static bool validateVOX(std::istream& input) {
	// read the signature
	unsigned char header[8];
	input.read((char *) header, 8);

	// check if reading went well
	if (!input.good()) {
		std::cerr << "!input.good()" << std::endl;
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

	if (numModels != 1) {
		throw std::runtime_error("VOX files with multiple models unsupported.");
	}

	// check that we have got a 'size' chunk
	assertChunkName(chunk, "SIZE", true);

	// read the sizes
	unsigned sizes[3];
	input.read((char *) sizes, 12);

	_width = sizes[0];
	_height = sizes[1];
	_depth = sizes[2];
	_voxels.resize(sizes[0] * sizes[1] * sizes[2]);

	// read the voxels
	readChunkHeader(input, chunk);
	assertChunkName(chunk, "XYZI", true);

	std::array<std::vector<ivec3>, 256> voxels;

	int numVoxels;
	input.read((char *) &numVoxels, 4);

	char data[4];

	for (int i = 0; i < numVoxels; i++) {
		input.read(data, 4);
		voxels[data[3]].emplace_back(data[0], data[1], data[2]);
	}

	std::array<Color, 256> palette = default_palette;

	if (input.eof()) {
		_Finish(voxels, palette);
	}

	throw std::runtime_error("");

}

void Image3D::_Finish(const std::array<std::vector<ivec3>, 256>& voxels, const std::array<Color, 256>& palette) {
	for (int i = 0; i < 256; i++) {
		Color c = palette[i];
		const std::vector<ivec3>& locations = voxels[i];

		for (ivec3 v : locations) {
			_voxels[v.x + v.y * _width + v.z * _width * _height] = c;
		}
	}
}

}
