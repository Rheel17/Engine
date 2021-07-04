/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "DebugLines.h"

#include "../Registry/Registry.h"

namespace rheel {

Model DebugLines::_single_line = Model(std::vector<model_vertex>{
		{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } }
}, std::vector<unsigned int>{ 0, 1 }, RenderType::LINES);

void DebugLines::SetLine(std::size_t idx, const DebugLines::line& l) {
	while (idx >= _debug_lines_entity->GetChildren().size()) {
		auto& child = _debug_lines_entity->AddChild();
		child.transform.SetScale(vec3{});
		child.AddComponent<ModelRenderComponent>(_single_line, Material());
	}

	auto* entity = _debug_lines_entity->GetChildren()[idx];
	entity->transform = Transform(l.start, vec3{}, l.end - l.start);
	entity->GetComponent<ModelRenderComponent>()->SetMaterial(Material{ l.color, 0.0f, 0.0f });
}

void DebugLines::OnActivate() {
	_debug_lines_entity = &GetEntity().AddChild();
}

void DebugLines::OnDeactivate() {
	GetEntity().RemoveChild(_debug_lines_entity);
}

void DebugLines::Update() {
	_debug_lines_entity->transform = Transform(glm::inverse(GetEntity().AbsoluteTransform().AsMatrix()));
}

}
