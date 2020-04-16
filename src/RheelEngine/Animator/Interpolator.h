/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_INTERPOLATOR_H
#define RHEELENGINE_INTERPOLATOR_H
#include "../_common.h"

#include <map>

namespace rheel {

template<typename T>
class RE_API Interpolator {

public:
	virtual ~Interpolator() = default;

	void AddPoint(float t, const T& value) {
		points.insert(t, value);
		t_min = std::min(t_min, t);
		t_max = std::max(t_max, t);
	}

	void AddPoint(float t, T&& value) {
		points.insert(t, std::forward<T>(value));
		t_min = std::min(t_min, t);
		t_max = std::max(t_max, t);
	}

	void RemovePoint(float t) {
		points.erase(t);

		if (points.empty()) {
			t_min = 0.0f;
			t_max = 0.0f;
		} else {
			t_min = points.begin()->first;
			t_max = (--points.end())->first;
		}
	}

	T operator()(float t) const {
		if (points.empty()) {
			return T{};
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

	virtual T GetValue(float t) const = 0;

	std::map<float, T> points;
	float t_min = 0;
	float t_max = 0;

};

}

#endif
