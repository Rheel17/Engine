/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGE_H_
#define IMAGE_H_
#include "../_common.h"

#include "../Color.h"

namespace rheel {

class RE_API Image {
	RE_NO_MOVE(Image);
	RE_NO_COPY(Image);

public:
	enum ImageFormat {
		FormatPNG
	};

public:
	/**
	 * Creates an empty image with the given dimensions
	 */
	Image(unsigned width, unsigned height);

	/**
	 * Creates the image based on an image file in the file system.
	 */
	Image(const std::string& filename);

	/**
	 * Creates the image based on an image file in the file system. This method
	 * overload allows for custom filename extensions.
	 */
	Image(const std::string& filename, ImageFormat format);

	/**
	 * Creates the image from memory.
	 */
	Image(const char *data, unsigned length, ImageFormat format);

	/**
	 * Creates the image from a std::istream&.
	 */
	Image(std::istream& input, ImageFormat format);

	unsigned Width() const;
	unsigned Height() const;

	const Color& At(unsigned x, unsigned y) const;

	const float *Data() const;

private:
	Image() = default;

	void _LoadPNG(std::istream& input);

	void _LoadFromFile(const std::string& filename);
	void _LoadFromFile(const std::string& filename, ImageFormat format);
	void _LoadFromMemory(const char *data, unsigned length, ImageFormat format);
	void _LoadFromStream(std::istream& input, ImageFormat format);

	unsigned _width = 0;
	unsigned _height = 0;

	std::vector<Color> _pixels;

};

}

#endif
