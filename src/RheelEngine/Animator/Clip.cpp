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

}