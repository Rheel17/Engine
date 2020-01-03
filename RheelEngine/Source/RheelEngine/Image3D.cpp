/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Image3D.h"

#include <fstream>
#include <sstream>

namespace rheel {

Image3D::Image3D(const std::string& filename) {
	_LoadFromFile(filename);
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
