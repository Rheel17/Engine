/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ANIMATORCOMPONENT_H
#define RHEELENGINE_ANIMATORCOMPONENT_H
#include "../_common.h"

#include "../Animator/Animator.h"
#include "../Component.h"

namespace rheel {

class RE_API AnimatorComponent : public Animator, public ComponentBase {

public:
	void Update() override;

};

}

#endif
