/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image.h"

#include <fstream>
#include <sstream>

#include "Renderer/ImageTexture.h"

namespace rheel {

Image::Image(unsigned width, unsigned height) :
		_width(width), _height(height) {

	_pixels.resize(width * height);
}

Image::Image(const std::string& filename) {
	_LoadFromFile(filename);
}

Image::Image(const std::string& filename, ImageFormat format) {
	_LoadFromFile(filename, format);
}

Image::Image(const char *data, unsigned length, ImageFormat format) {
	_LoadFromMemory(data, length, format);
}

Image::Image(std::istream& input, ImageFormat format) {
	_LoadFromStream(input, format);
}

unsigned Image::Width() const {
	return _width;
}

unsigned Image::Height() const {
	return _height;
}

const Color& Image::At(unsigned x, unsigned y) const {
	return _pixels[y * _width + x];
}

const float *Image::Data() const {
	return reinterpret_cast<const float *>(_pixels.data());
}

void Image::_LoadFromFile(const std::string& filename) {
	auto pos = filename.find_last_of('.');

	if (pos == std::string::npos) {
		throw std::runtime_error("Error while reading image file (cannot find file extension): " + filename);
	}

	std::string extension = filename.substr(pos + 1);

	if (extension == "png") {
		_LoadFromFile(filename, FormatPNG);
		return;
	}

	throw std::runtime_error("Error while reading image file (file extension not supported): " + filename);
}

void Image::_LoadFromFile(const std::string& filename, ImageFormat format) {
	std::ifstream f(filename, std::ios::binary);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + filename);
	}

	_LoadFromStream(f, format);
}

void Image::_LoadFromMemory(const char *data, unsigned length, ImageFormat format) {
	std::stringstream input(std::ios::binary | std::ios::in | std::ios::out);
	input.write(data, length);
	input.seekg(0);

	_LoadFromStream(input, format);
}

void Image::_LoadFromStream(std::istream& input, ImageFormat format) {
	switch (format) {
		case FormatPNG:
			_LoadPNG(input);
			break;
	}
}

}
