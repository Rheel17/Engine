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

class SequenceBase {

public:
	virtual ~SequenceBase() = default;

	virtual void Update(float t) = 0;

	virtual float GetMaxTime() const = 0;

};

template<typename V, typename Setter, typename InterpolatorImpl>
class Sequence : public SequenceBase {
	friend class Clip;

	using IImpl = std::remove_reference_t<InterpolatorImpl>;

private:
	Sequence(Setter setter, const std::remove_reference_t<IImpl>& interpolator) :
			_setter(std::move(setter)),
			_interpolator(interpolator) {}

	Sequence(Setter setter, std::remove_reference_t<IImpl>&& interpolator) :
			_setter(std::move(setter)),
			_interpolator(std::forward<IImpl>(interpolator)) {}

public:
	IImpl& GetInterpolator() {
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

	void Update(float t) override {
		_setter(_interpolator(t));
	}

	float GetMaxTime() const override {
		return _interpolator.GetMaxTime();
	}

private:
	Setter _setter;
	IImpl _interpolator;

};

}

#endif
