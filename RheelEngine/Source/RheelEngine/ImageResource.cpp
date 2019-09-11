#include "ImageResource.h"

namespace rheel {

ImageResource::ImageResource(const std::string& path) :
		Resource(path, new Image(path)) {}

ImageResource::ImageResource(const std::string& path, Image *image) :
		Resource(path, image) {}

ImageResource::~ImageResource() {
	delete _image_texture;
}

const ImageTexture& ImageResource::GetImageTexture() const {
	if (!_image_texture) {
		_image_texture = new ImageTexture(Get());
	}

	return *_image_texture;
}



}
