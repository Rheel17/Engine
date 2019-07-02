#ifndef IMAGE_H_
#define IMAGE_H_
#include "_common.h"

#include <memory>

namespace rheel {

class Image;
using ImagePtr = std::shared_ptr<Image>;

class RE_API Image {

public:
	enum ImageFormat {
		FormatPNG
	};

public:
	/**
	 * Creates the image based on a image file in the file system.
	 */
	static ImagePtr LoadFromFile(const std::string& filename);

	/**
	 * Creates the image from memory.
	 */
	static ImagePtr LoadFromMemory(const char *data, ImageFormat format);

private:


};

}

#endif
