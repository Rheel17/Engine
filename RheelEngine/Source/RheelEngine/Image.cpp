#include "Image.h"

#include <fstream>
#include <sstream>

#include "Renderer/ImageTexture.h"

namespace rheel {

Image::Image(unsigned width, unsigned height) :
		_width(width), _height(height) {

	_pixels.resize(width * height);
}

Image::~Image() {
	ImageTexture::_DestroyTexture(this);
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

ImagePtr Image::LoadFromFile(const std::string& filename) {
	auto pos = filename.find_last_of('.');

	if (pos == std::string::npos) {
		throw std::runtime_error("Error while reading image file (cannot find file extension): " + filename);
	}

	std::string extension = filename.substr(pos + 1);

	if (extension == "png") {
		return LoadFromFile(filename, FormatPNG);
	}

	throw std::runtime_error("Error while reading image file (file extension not supported): " + filename);
}

ImagePtr Image::LoadFromFile(const std::string& filename, ImageFormat format) {
	std::ifstream f(filename);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + filename);
	}

	return LoadFromStream(f, format);
}

ImagePtr Image::LoadFromMemory(const char *data, unsigned length, ImageFormat format) {
	std::stringstream input(std::ios::binary | std::ios::in | std::ios::out);
	input.write(data, length);
	input.seekg(0);

	return LoadFromStream(input, format);
}

ImagePtr Image::LoadFromStream(std::istream& input, ImageFormat format) {
	auto image = std::shared_ptr<Image>(new Image());

	switch (format) {
		case FormatPNG:
			image->_LoadPNG(input);
			break;
	}

	return image;
}

}
