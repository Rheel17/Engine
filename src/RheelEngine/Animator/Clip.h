/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CLIP_H
#define RHEELENGINE_CLIP_H
#include "../_common.h"

#include "Sequence.h"

namespace rheel {

class RE_API Clip {

public:
	/**
	 * Adds a sequence to this clip. A sequence is the changing of a single
	 * variable or function value over time. Use the overload
	 * AddSequence(V&, Interpolator<V>) to sequence a reference variable.
	 */
	template<typename V, typename Setter, typename InterpolatorImpl>
	Sequence<V, Setter, InterpolatorImpl>& AddSequence(Setter setter, const InterpolatorImpl& interpolator) {
		auto ptr = new Sequence<V, Setter, InterpolatorImpl>(setter, interpolator);
		_sequences.push_back(std::unique_ptr<SequenceBase>(ptr));

		return *ptr;
	}

	/**
	 * Adds a sequence to this clip. A sequence is the changing of a single
	 * variable or function value over time. Use the overload
	 * AddSequence(V&, Interpolator<V>) to sequence a reference variable.
	 */
	template<typename V, typename Setter, typename InterpolatorImpl>
	Sequence<V, Setter, InterpolatorImpl>& AddSequence(Setter setter, InterpolatorImpl&& interpolator) {
		auto ptr = new Sequence<V, Setter, InterpolatorImpl>(setter, std::forward<InterpolatorImpl>(interpolator));
		_sequences.push_back(std::unique_ptr<SequenceBase>(ptr));

		return *ptr;
	}

	/**
	 * Adds a variable sequence to this clip. A sequence is the changing of a
	 * single variable value over time. Make sure that this reference stays
	 * valid over the life-time of the sequence.
	 */
	template<typename V, typename InterpolatorImpl>
	Sequence<V, ReferenceSetter<V>, InterpolatorImpl>& AddSequence(V& reference, const InterpolatorImpl& interpolator) {
		return AddSequence<V, ReferenceSetter<V>>(ReferenceSetter<V>(reference), interpolator);
	}

	/**
	 * Adds a variable sequence to this clip. A sequence is the changing of a
	 * single variable value over time. Make sure that this reference stays
	 * valid over the life-time of the sequence.
	 */
	template<typename V, typename InterpolatorImpl>
	Sequence<V, ReferenceSetter<V>, InterpolatorImpl>& AddSequence(V& reference, InterpolatorImpl&& interpolator) {
		return AddSequence<V, ReferenceSetter<V>>(ReferenceSetter<V>(reference), std::forward<InterpolatorImpl>(interpolator));
	}

	/**
	 * Removes a sequence from this clip.
	 */
	template<typename V, typename Setter, typename InterpolatorImpl>
	void RemoveSequence(Sequence<V, Setter, InterpolatorImpl>& sequence) {
		_sequences.erase(std::find_if(_sequences.begin(), _sequences.end(), [sequence](const std::unique_ptr<SequenceBase>& ptr) {
			return ptr.get() == &sequence;
		}));
	}

	/**
	 * Removes all sequences from this clip.
	 */
	void Clear();

	/**
	 * Updates all the sequences with the specified time.
	 */
	void Update(float t);

	/**
	 * Returns the last time value for which an animation value is defined.
	 * Update invocations beyond this time are valid, but will give the same
	 * results as invocing with the maximum time.
	 * @return
	 */
	float GetMaxTime() const;

private:
	std::vector<std::unique_ptr<SequenceBase>> _sequences;

};

}

#endif
