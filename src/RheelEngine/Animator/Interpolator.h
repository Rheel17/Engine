/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_INTERPOLATOR_H
#define RHEELENGINE_INTERPOLATOR_H
#include "../_common.h"

#include <map>

namespace rheel {

template<typename V>
class Interpolator {

public:
	virtual ~Interpolator() = default;

	virtual void AddPoint(float t, const V& value) {
		points[t] = value;
		t_min = std::min(t_min, t);
		t_max = std::max(t_max, t);
	}

	virtual void AddPoint(float t, V&& value) {
		points[t] = std::forward<V>(value);
		t_min = std::min(t_min, t);
		t_max = std::max(t_max, t);
	}

	virtual void RemovePoint(float t) {
		points.erase(t);

		if (points.empty()) {
			t_min = 0.0f;
			t_max = 0.0f;
		} else {
			t_min = points.begin()->first;
			t_max = (--points.end())->first;
		}
	}

	V operator()(float t) const {
		if (points.empty()) {
			return V{};
		}

		if (points.size() == 1 || t <= this->t_min) {
			return points.begin()->second;
		}

		if (t >= this->t_max) {
			return (--points.end())->second;
		}

		return GetValue(t);
	}

protected:
	Interpolator() = default;

	virtual V GetValue(float t) const = 0;

	std::map<float, V> points;
	float t_min = 0;
	float t_max = 0;

};

}

#endif
