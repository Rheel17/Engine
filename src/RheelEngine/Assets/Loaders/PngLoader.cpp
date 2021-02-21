/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "PngLoader.h"

#include <png.h>

#include <fstream>

#define FLOAT(x) ((x) / 255.0f)

namespace rheel {

static bool validate_png(std::istream& input) {
	// read the signature
	std::array<png_byte, 8> signature{};
	input.read(reinterpret_cast<char*>(signature.data()), 8); // NOLINT (safe)

	// check if reading went well
	if (!input.good()) {
		return false;
	}

	// compare the signature
	return png_sig_cmp(signature.data(), 0, 8) == 0;
}

Image PngLoader::Load(const std::string& path) const {
	std::ifstream f(path, std::ios::binary);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + path);
	}

	return _load_png(f);
}

Image PngLoader::_load_png(std::istream& input) {
	if (!validate_png(input)) {
		throw std::runtime_error("An error occurred while reading PNG file: PNG signature not valid.");
	}

	// create the read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		throw std::runtime_error("Failed to create png_read_struct.");
	}

	// create the info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		throw std::runtime_error("Failed to create png_info_struct.");
	}

	// declare the pointers for reading
	png_bytep* rows = nullptr;
	unsigned char* data = nullptr;

	// jump here if something goes wrong in the parsing.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		delete[] rows;
		delete[] data;
		throw std::runtime_error("An error occurred while reading the PNG file.");
	}

	// set the custom read function
	png_set_read_fn(png_ptr, static_cast<png_voidp>(&input),
			[](png_structp png_ptr, png_bytep data, png_size_t length) {

				auto* stream = static_cast<std::istream*>(png_get_io_ptr(png_ptr));
				stream->read(reinterpret_cast<char*>(data), length); // NOLINT (safe)
			});

	// we've already read the header, so skip the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read the info
	png_read_info(png_ptr, info_ptr);

	// parse the png info
	auto width = png_get_image_width(png_ptr, info_ptr);
	auto height = png_get_image_height(png_ptr, info_ptr);

	auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	auto color_type = png_get_color_type(png_ptr, info_ptr);

	// set the conversion to rgba with 8 bits per channel
	if (bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}

	switch (color_type) {
		case PNG_COLOR_TYPE_GRAY:
			if (bit_depth < 8) {
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}
			// Fallthrough
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(png_ptr);
			break;
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png_ptr);
			break;
		default:
			break;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	// no alpha channel supplied: fill with 0xFF
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
	}

	png_read_update_info(png_ptr, info_ptr);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"

	size_t stride = png_get_rowbytes(png_ptr, info_ptr);
	rows = new png_bytep[height];
	data = new unsigned char[height * stride];

	for (unsigned i = 0; i < height; i++) {
		rows[i] = static_cast<png_bytep>(data) + stride * i;
	}

	// read the image
	png_read_image(png_ptr, rows);

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

#pragma clang diagnostic pop

	// delete the png handle and the temporary buffers
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	delete[] rows;
	delete[] data;

	return Image(width, height, std::move(pixels));
}

}
