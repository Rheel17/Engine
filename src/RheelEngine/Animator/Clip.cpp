/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Clip.h"

namespace rheel {

void Clip::Clear() {
	_sequences.clear();
}

void Clip::Update(float t) {
	for (const auto& sequence : _sequences) {
		sequence->Update(t);
	}
}

float Clip::GetMaxTime() const {
	auto iter = std::max_element(_sequences.cbegin(), _sequences.cend(), [](const auto& s1, const auto& s2) {
		return s1->GetMaxTime() < s2->GetMaxTime();
	});

	return iter->get()->GetMaxTime();
}

}