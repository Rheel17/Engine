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
		_points.insert(t, value);
		_t_min = std::min(_t_min, t);
		_t_max = std::max(_t_max, t);
	}

	void AddPoint(float t, T&& value) {
		_points.insert(t, std::forward<T>(value));
		_t_min = std::min(_t_min, t);
		_t_max = std::max(_t_max, t);
	}

	void RemovePoint(float t) {
		_points.erase(t);

		if (_points.empty()) {
			_t_min = 0.0f;
			_t_max = 0.0f;
		} else {
			_t_min = _points.begin()->first;
			_t_max = (--_points.end())->first;
		}
	}

	T operator()(float t) const {
		if (_points.empty()) {
			return T{};
		}

		if (_points.size() == 1 || t <= this->_t_min) {
			return _points.begin()->second;
		}

		if (t >= this->_t_max) {
			return (--_points.end())->second;
		}

		return _GetValue(t);
	}

protected:
	Interpolator() = default;

	virtual T _GetValue(float t) const = 0;

	std::map<float, T> _points;
	float _t_min = 0;
	float _t_max = 0;

};

}

#endif
