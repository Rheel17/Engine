/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_DEBUGLINES_H
#define ENGINE_DEBUGLINES_H
#include "../_common.h"

#include "ModelRenderComponent.h"

namespace rheel {

class DebugLines : public Component {

public:
	// gen_component_id
	static constexpr const ComponentId id = 65282;

	struct line {
		vec3 start;
		vec3 end;
		Color color;
	};

	void SetLine(std::size_t idx, const line& l);

protected:
	void OnActivate() override;
	void OnDeactivate() override;

public:
	void Update() override;

private:
	Entity* _debug_lines_entity{};
	static Model _single_line;
};

}

#endif
