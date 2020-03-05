/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SKYBOX_H
#define RHEELENGINE_SKYBOX_H
#include "../_common.h"

#include "../Component.h"

namespace rheel {

class RE_API Skybox : public ComponentBase {

public:
	explicit Skybox(Image image, float scale = 1.0f);

	void Activate() override;

	void Deactivate() override;

	const Image& GetImage() const;

	float GetScale() const;

private:
	Image _image;
	float _scale;

};

}

#endif
