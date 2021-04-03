/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_LIGHT_H
#define RHEELENGINE_LIGHT_H
#include "../_common.h"

#include "../Color.h"
#include "../Component.h"

namespace rheel {

class RE_API Light : public Component {

public:
	// gen_component_id
	static constexpr const ComponentId id = 65284;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

	void SetColor(const Color& color);
	const Color& GetColor() const;

	void SetShadowDistance(float shadow_distance);
	float ShadowDistance() const;
	bool CastsShadows() const;

protected:
	explicit Light(const Color& color);
	Light(const Color& color, float shadow_distance);

private:
	Color _color;
	float _shadow_distance;

};

}

#endif
