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
	template<typename V, typename Setter>
	Sequence<V, Setter>& AddSequence(Setter setter, Interpolator<V> interpolator) {
		auto ptr = std::unique_ptr<Sequence<V, Setter>>(new Sequence<V, Setter>(setter, std::move(interpolator)));
		_sequences.push_back(ptr);

		return *ptr;
	}

	/**
	 * Adds a variable sequence to this clip. A sequence is the changing of a
	 * single variable value over time. Make sure that this reference stays
	 * valid over the life-time of the sequence.
	 */
	template<typename V>
	Sequence<V, ReferenceSetter<V>>& AddSequence(V& reference, Interpolator<V> interpolator) {
		return AddSequence<V, ReferenceSetter<V>>(ReferenceSetter<V>(reference), std::move(interpolator));
	}

	/**
	 * Removes a sequence from this clip.
	 */
	template<typename V, typename Setter>
	void RemoveSequence(Sequence<V, Setter>& sequence) {
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

private:
	std::vector<std::unique_ptr<SequenceBase>> _sequences;

};

}

#endif
