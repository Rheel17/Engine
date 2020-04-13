/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PngLoader.h"

#include <png.h>

#include <fstream>

#define FLOAT(x) ((x) / 255.0f)
#define BYTE(x) ((png_byte) ((x) * 255))

namespace rheel {

static bool validatePNG(std::istream& input) {
	// read the signature
	png_byte signature[8];
	input.read((char *) signature, 8);

	// check if reading went well
	if (!input.good()) {
		return false;
	}

	// compare the signature
	return png_sig_cmp(signature, 0, 8) == 0;
}

Image PngLoader::_DoLoad(const std::string& path) const {
	std::ifstream f(path, std::ios::binary);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + path);
	}

	return _LoadPNG(f);
}

Image PngLoader::_LoadPNG(std::istream& input) {
	if (!validatePNG(input)) {
		throw std::runtime_error("An error occurred while reading PNG file: PNG signature not valid.");
	}

	// create the read struct
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!pngPtr) {
		throw std::runtime_error("Failed to create png_read_struct.");
	}

	// create the info struct
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		throw std::runtime_error("Failed to create png_info_struct.");
	}

	// declare the pointers for reading
	png_bytep *rows = nullptr;
	unsigned char *data = nullptr;

	// jump here if something goes wrong in the parsing.
	if (setjmp(png_jmpbuf(pngPtr))) {
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) nullptr);
		delete[] rows;
		delete[] data;
		throw std::runtime_error("An error occurred while reading the PNG file.");
	}

	// set the custom read function
	png_set_read_fn(pngPtr, static_cast<png_voidp>(&input),
			[](png_structp pngPtr, png_bytep data, png_size_t length) {

		auto stream = static_cast<std::istream*>(png_get_io_ptr(pngPtr));
		stream->read((char *) data, length);
	});

	// we've already read the header, so skip the first 8 bytes
	png_set_sig_bytes(pngPtr, 8);

	// read the info
	png_read_info(pngPtr, infoPtr);

	// parse the png info
	auto width = png_get_image_width(pngPtr, infoPtr);
	auto height = png_get_image_height(pngPtr, infoPtr);

	auto bitDepth = png_get_bit_depth(pngPtr, infoPtr);
	auto colorType = png_get_color_type(pngPtr, infoPtr);

	// set the conversion to rgba with 8 bits per channel
	if (bitDepth == 16) {
		png_set_strip_16(pngPtr);
	}

	switch (colorType) {
		case PNG_COLOR_TYPE_GRAY:
			if (bitDepth < 8) {
				png_set_expand_gray_1_2_4_to_8(pngPtr);
			}
			// Fallthrough
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(pngPtr);
			break;
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(pngPtr);
			break;
		default:
			break;
	}

	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(pngPtr);
	}

	// no alpha channel supplied: fill with 0xFF
	if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) {
		png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);
	}

	png_read_update_info(pngPtr, infoPtr);

	size_t stride = png_get_rowbytes(pngPtr, infoPtr);
	rows = new png_bytep[height];
	data = new unsigned char[height * stride];

	for (unsigned i = 0; i < height; i++) {
		rows[i] = (png_bytep) data + stride * i;
	}

	// read the image
	png_read_image(pngPtr, rows);

	// create the actual image class storage
	std::vector<Color> pixels;
	pixels.resize(width * height);

	// upload the data from the temporary buffer to the class storage
	for (unsigned pixel = 0; pixel < width * height; pixel++) {
		pixels[pixel] = {
				FLOAT(data[4 * pixel + 0]),
				FLOAT(data[4 * pixel + 1]),
				FLOAT(data[4 * pixel + 2]),
				FLOAT(data[4 * pixel + 3]),
		};
	}

	// delete the png handle and the temporary buffers
	png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) nullptr);
	delete[] rows;
	delete[] data;

	return Image(width, height, std::move(pixels));
}

}
