#ifndef LIGHT_H_
#define LIGHT_H_
#include "_common.h"

#include "Color.h"

namespace rheel {

class RE_API Light {
	using Color_t = rheel::Color;

public:
	enum LightType {
		PointLight = 0, SpotLight = 1, DirectionalLight = 2
	};

public:
	Light(vec3 position, Color_t color, float attenuation, bool castsShadows);
	Light(vec3 position, Color_t color, vec3 direction, float spotAttenuation, float attenuation, bool castsShadows);
	Light(Color_t color, vec3 direction, bool castsShadows);

	LightType Type() const;
	const vec3& Position() const;
	const vec3& Direction() const;
	const Color_t& Color() const;
	float Attenuation() const;
	float SpotAttenuation() const;
	bool CastsShadows() const;

	void SetPosition(const vec3& position);
	void SetDirection(const vec3& direction);
	void SetColor(const Color_t& color);
	void SetAttenuation(float attenutation);
	void SetSpotAttenuation(float spotAttenuation);
	void SetCastsShadows(bool castsShadows);

private:
	LightType _type;
	vec3 _position;
	vec3 _direction;
	Color_t _color;
	float _attenuation = 0.0f;
	float _spot_attenuation = 0.0f;
	bool _casts_shadows = false;

};

}

#endif
