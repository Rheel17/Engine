/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image3D.h"

#include <fstream>
#include <sstream>

namespace rheel {

Image3D::Image3D(unsigned xSize, unsigned ySize, unsigned zSize) :
		_width(xSize), _height(ySize), _depth(zSize) {

	_voxels.resize(0);
}

Image3D::Image3D(const std::string& filename) {
	_LoadFromFile(filename);
}

unsigned Image3D::GetWidth() const {
	return _width;
}

unsigned Image3D::GetHeight() const {
	return _height;
}

unsigned Image3D::GetDepth() const {
	return _depth;
}

const Color &Image3D::At(unsigned x, unsigned y, unsigned z) const {
	return _voxels[x + y * _width + z * _width * _height];
}

void Image3D::_LoadFromFile(const std::string& filename) {
	std::ifstream f(filename, std::ios::binary);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + filename);
	}

	_LoadFromStream(f);
}

void Image3D::_LoadFromStream(std::istream& input) {
	_LoadVOX(input);
}

}
