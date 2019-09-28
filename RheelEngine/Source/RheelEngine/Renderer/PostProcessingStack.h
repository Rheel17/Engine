/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef POSTPROCESSINGSTACK_H_
#define POSTPROCESSINGSTACK_H_
#include "../_common.h"

namespace rheel {

class RE_API PostProcessingStack {

public:
	struct Bloom {
		float threshold_start;
		float threshold_end;
		float multiplier;
		unsigned level;
	};

public:
	void SetBloom(float thresholdStart, float thresholdEnd, float multiplier, unsigned level);
	void SetBloomOff();
	const Bloom& GetBloom() const;

private:
	bool _has_bloom;
	Bloom _bloom;

};

}

#endif
