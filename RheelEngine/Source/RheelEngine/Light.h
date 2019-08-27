#ifndef LIGHT_H_
#define LIGHT_H_
#include "_common.h"

#include "Color.h"

#include <memory>

namespace rheel {

class RE_API Light {

protected:
	using Color_t = rheel::Color;

public:
	virtual ~Light();

	void SetColor(const Color_t& color);
	const Color_t& Color() const;

	void SetShadowDistance(float shadowDistance);
	float ShadowDistance() const;
	bool CastsShadows() const;

protected:
	Light(const Color_t& color);
	Light(const Color_t& color, float shadowDistance);

private:
	Color_t _color;
	float _shadow_distance;

};

using LightPtr = std::shared_ptr<Light>;

}

#endif
