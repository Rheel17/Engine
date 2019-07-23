#ifndef IMAGE_H_
#define IMAGE_H_
#include "_common.h"

#include <vector>
#include <memory>

#include "Color.h"

namespace rheel {

class Image;
using ImagePtr = std::shared_ptr<Image>;

class RE_API Image {

public:
	enum ImageFormat {
		FormatPNG
	};

public:
	Image(unsigned width, unsigned height);
	~Image();

	unsigned Width() const;
	unsigned Height() const;

	const Color& At(unsigned x, unsigned y) const;

	const float *Data() const;

private:
	Image() = default;

	void _LoadPNG(std::istream& input);

	unsigned _width = 0;
	unsigned _height = 0;

	std::vector<Color> _pixels;

public:
	/**
	 * Creates the image based on an image file in the file system.
	 */
	static ImagePtr LoadFromFile(const std::string& filename);

	/**
	 * Creates the image based on an image file in the file system. This method
	 * overload allows for custom filename extensions.
	 */
	static ImagePtr LoadFromFile(const std::string& filename, ImageFormat format);

	/**
	 * Creates the image from memory.
	 */
	static ImagePtr LoadFromMemory(const char *data, unsigned length, ImageFormat format);

	/**
	 * Creates the image from a std::istream&.
	 */
	static ImagePtr LoadFromStream(std::istream& input, ImageFormat format);

};

}

#endif
