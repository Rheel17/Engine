/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SKYBOX_H
#define RHEELENGINE_SKYBOX_H
#include "../_common.h"

#include "../Component.h"
#include "../Assets/Image.h"

namespace rheel {

class RE_API Skybox : public Component {

public:
	// gen_component_id

	explicit Skybox(std::array<Image, 6> images, float scale = 1.0f);

	explicit Skybox(const Image& image, float scale = 1.0f);

	const std::array<Image, 6>& GetImages() const;

	float GetScale() const;

private:
	std::array<Image, 6> _images;
	float _scale;

private:
	static std::array<Image, 6> LoadImages_(const Image& image);
	static Image LoadImage_(const Image& image, int part);

};

}

#endif
