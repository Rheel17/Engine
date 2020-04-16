/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CUBICINTERPOLATOR_H
#define RHEELENGINE_CUBICINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename T>
class RE_API CubicInterpolator : public TransitionInterpolator<T> {

public:
	CubicInterpolator() :
			TransitionInterpolator<T>(Transition::CUBIC) {}

};

}

#endif
