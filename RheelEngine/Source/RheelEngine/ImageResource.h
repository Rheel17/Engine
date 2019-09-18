#ifndef IMAGERESOURCE_H_
#define IMAGERESOURCE_H_
#include "_common.h"

#include "Image.h"
#include "Resource.h"
#include "Renderer/ImageTexture.h"

namespace rheel {

class RE_API ImageResource : public Resource<Image> {
	RE_NO_COPY(ImageResource);
	RE_NO_MOVE(ImageResource);

	friend class ResourceManager;

public:
	const ImageTexture& GetImageTexture() const;

private:
	ImageResource(const std::string& path);
	ImageResource(const std::string& path, Image *image);

	mutable std::unique_ptr<ImageTexture> _image_texture = nullptr;

};

}

#endif
