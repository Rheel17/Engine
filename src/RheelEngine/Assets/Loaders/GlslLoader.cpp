/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "GlslLoader.h"

#include <fstream>
#include <streambuf>

namespace rheel {

Shader rheel::GlslLoader::DoLoad(const std::string& path) const {
	std::ifstream f(path);

	if (!f) {
		throw std::runtime_error("Error while reading shader file: " + path);
	}

	return Shader(std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>()));
}

}