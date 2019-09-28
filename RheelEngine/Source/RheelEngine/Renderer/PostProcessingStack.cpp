/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "PostProcessingStack.h"

namespace rheel {

void PostProcessingStack::SetBloom(float thresholdStart, float thresholdEnd, float multiplier, unsigned level) {
	_has_bloom = true;
	_bloom = { thresholdStart, thresholdEnd, multiplier, level };
}

void PostProcessingStack::SetBloomOff() {
	_has_bloom = false;
	_bloom = Bloom {};
}

const PostProcessingStack::Bloom& PostProcessingStack::GetBloom() const {
	return _bloom;
}

}
