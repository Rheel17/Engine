/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_COSINEINTERPOLATOR_H
#define RHEELENGINE_COSINEINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename T>
class RE_API CosineInterpolator : public TransitionInterpolator<T> {

public:
	CosineInterpolator() :
			TransitionInterpolator<T>(Transition::cosine) {}

};

}

#endif
