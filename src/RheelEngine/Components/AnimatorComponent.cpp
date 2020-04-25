/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "AnimatorComponent.h"

namespace rheel {

void AnimatorComponent::Update() {
	Animator::Update(GetTimeDelta());
}

}