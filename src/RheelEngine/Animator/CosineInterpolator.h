/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_COSINEINTERPOLATOR_H
#define RHEELENGINE_COSINEINTERPOLATOR_H
#include "../_common.h"

#include "TransitionInterpolator.h"

namespace rheel {

template<typename V>
class CosineInterpolator : public TransitionInterpolator<V> {

public:
	CosineInterpolator() :
			TransitionInterpolator<V>(Transition::cosine) {}

};

}

#endif
