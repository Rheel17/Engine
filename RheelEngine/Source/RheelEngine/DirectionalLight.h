#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_
#include "_common.h"

#include "Light.h"

namespace rheel {

class RE_API DirectionalLight : public Light {

public:
	DirectionalLight(const Color_t& color, const vec3& direction);

	~DirectionalLight();

	void SetDirection(const vec3& direction);
	const vec3& Direction() const;

private:
	vec3 _direction;

};

}

#endif
