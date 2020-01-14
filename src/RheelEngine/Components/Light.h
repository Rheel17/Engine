/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef LIGHT_H_
#define LIGHT_H_
#include "../_common.h"

#include "../Color.h"
#include "../Component.h"

namespace rheel {

class RE_API Light : public ComponentBase {

public:
	virtual ~Light();

	void Activate() override;

	void SetColor(const Color& color);
	const Color& GetColor() const;

	void SetShadowDistance(float shadowDistance);
	float ShadowDistance() const;
	bool CastsShadows() const;


protected:
	Light(const Color& color);
	Light(const Color& color, float shadowDistance);

private:
	Color _color;
	float _shadow_distance;

};

}

#endif
