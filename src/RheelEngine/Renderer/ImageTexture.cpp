/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ImageTexture.h"

namespace rheel {

void ImageTexture::Bind(unsigned textureUnit) const {
	_texture.Bind(textureUnit);
}

ImageTexture::ImageTexture(Image image)
		: _texture(image.GetWidth(), image.GetHeight(), GL_RGBA) {

	_texture.SetMinifyingFilter(GL::FilterFunction::LINEAR);
	_texture.SetMagnificationFilter(GL::FilterFunction::LINEAR);
	_texture.SetWrapParameterS(GL::WrapParameter::REPEAT);
	_texture.SetWrapParameterT(GL::WrapParameter::REPEAT);

	unsigned w = image.GetWidth(), h = image.GetHeight();
	const float *data = image.GetRawColorData();
	auto glData = new float[w * h * 4];

	// reverse the y-coordinate of the image for OpenGL.
	for (unsigned y = 0; y < h; y++) {
		memcpy(glData + y * w * 4, data + (h - y - 1) * w * 4, w * 4 * sizeof(float));
	}

	_texture.SetData(GL_RGBA, GL_FLOAT, glData);

	delete[] glData;
}

}
