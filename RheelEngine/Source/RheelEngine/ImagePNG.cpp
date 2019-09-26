/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Image.h"

#include <png.h>

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

void Image::_LoadPNG(std::istream& input) {
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
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) 0);
		delete[] rows;
		delete[] data;
		throw std::runtime_error("An error occurred while reading the PNG file.");
	}

	// set the custom read function
	png_set_read_fn(pngPtr, static_cast<png_voidp>(&input),
			[](png_structp pngPtr, png_bytep data, png_size_t length) {

		std::istream *stream = static_cast<std::istream*>(png_get_io_ptr(pngPtr));
		stream->read((char *) data, length);
	});

	// we've already read the header, so skip the first 8 bytes
	png_set_sig_bytes(pngPtr, 8);

	// read the info
	png_read_info(pngPtr, infoPtr);

	// parse the png info
	_width = png_get_image_width(pngPtr, infoPtr);
	_height = png_get_image_height(pngPtr, infoPtr);

	unsigned int bitDepth = png_get_bit_depth(pngPtr, infoPtr);
	unsigned int channels = png_get_channels(pngPtr, infoPtr);
	unsigned int colorType = png_get_color_type(pngPtr, infoPtr);

	// set the conversion to rgba with 8 bits per channel
	switch(colorType) {
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(pngPtr);
			channels = 3;
			break;
		case PNG_COLOR_TYPE_GRAY:
			if (bitDepth < 8) {
				png_set_expand_gray_1_2_4_to_8(pngPtr);
				bitDepth = 8;
			}
			break;
	}

	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(pngPtr);
		channels++;
	}

	if (bitDepth == 16) {
		png_set_strip_16(pngPtr);
	}

	png_read_update_info(pngPtr, infoPtr);

	// check if we have a readable image for this class
	if (!(channels == 1 || channels == 3 || channels == 4)) {
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) 0);
		throw std::runtime_error("Invalid number of channels. Can only work with 1, 3, or 4.");
	}

	// create the reading pointers
	unsigned stride = _width * bitDepth * channels / 8;
	rows = new png_bytep[_height];
	data = new unsigned char[_height * stride];

	for (unsigned i = 0; i < _height; i++) {
		rows[i] = (png_bytep) data + stride * i;
	}

	// read the image
	png_read_image(pngPtr, rows);

	// create the actual image class storage
	_pixels.resize(_width * _height);

	// upload the data from the temporary buffer to the class storage
	for (unsigned pixel = 0; pixel < _width * _height; pixel++) {
		switch (channels) {
			case 1: // greyscale
				_pixels[pixel] = {
						FLOAT(data[pixel]),
						FLOAT(data[pixel]),
						FLOAT(data[pixel]),
						1.0f
				};
				break;

			case 3: // RGB
				_pixels[pixel] = {
						FLOAT(data[3 * pixel + 0]),
						FLOAT(data[3 * pixel + 1]),
						FLOAT(data[3 * pixel + 2]),
						1.0f
				};
				break;

			case 4: // RGBA
				_pixels[pixel] = {
						FLOAT(data[4 * pixel + 0]),
						FLOAT(data[4 * pixel + 1]),
						FLOAT(data[4 * pixel + 2]),
						FLOAT(data[4 * pixel + 3]),
				};
				break;
		}
	}

	// delete the png handle and the temporary buffers
	png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) 0);
	delete[] rows;
	delete[] data;
}

}
