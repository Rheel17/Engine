/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_SEQUENCE_H
#define RHEELENGINE_SEQUENCE_H
#include "../_common.h"

#include "Interpolator.h"

namespace rheel {

template<typename V>
class ReferenceSetter {

public:
	ReferenceSetter(V& reference) :
			_reference(reference) {}

	void operator()(const V& v) {
		_reference = v;
	}

	void operator()(V&& v) {
		_reference = std::forward<V>(v);
	}

private:
	V& _reference;

};

class SequenceBase {};

template<typename V, typename Setter>
class Sequence : public SequenceBase {

private:
	Sequence(Setter setter, Interpolator<V> interpolator) :
			_setter(std::move(setter)),
			_interpolator(std::move(interpolator)) {}

public:
	Interpolator<V>& GetInterpolator() {
		return _interpolator;
	}

	virtual void AddPoint(float t, const V& value) {
		_interpolator.AddPoint(t, value);
	}

	virtual void AddPoint(float t, V&& value) {
		_interpolator.AddPoint(t, std::forward<V>(value));
	}

	virtual void RemovePoint(float t) {
		_interpolator.RemovePoint(t);
	}

	void Update(float t) {
		_setter(_interpolator(t));
	}

private:
	Setter _setter;
	Interpolator<V> _interpolator;

};

}

#endif
