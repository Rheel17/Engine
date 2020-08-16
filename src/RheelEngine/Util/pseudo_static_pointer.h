/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_PSEUDO_STATIC_POINTER_H
#define RHEELENGINE_PSEUDO_STATIC_POINTER_H
#include "../_common.h"

namespace rheel {

/**
 * The pseudo_static type helps in implementing pseudo-static variables; where
 * the variable is the same for all instances of the same class, and its
 * life-time is equal to that of the union of the life-times of the instances.
 *
 * Note: Due to the nature of the implementation, it is advised to use a private
 * internal class as type for the pseudo_static field. This will ensure that the
 * variable is unique to the owning class.
 *
 * While it is possible to modify the internal value by retrieving a non-const
 * reference with value(), this is not recommended. If all instances of the
 * pseudo_static variable are destroyed, the value and its modifications will
 * be destroyed. Any new instances will cause the value to be
 * default-constructed again.
 */
template<typename T>
struct pseudo_static_pointer {

public:
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

	/**
	 * Constructs the pseudo-static variable. If this is the only instance, the
	 * variable will be constructed.
	 */
	pseudo_static_pointer() {
		_increase_ref_count();
	}

	/**
	 * Destructs the pseudo-static variable. If this was the only instance, the
	 * variable will be destructed.
	 */
	~pseudo_static_pointer() {
		_decrease_ref_count();
	}

	pseudo_static_pointer(const pseudo_static_pointer&) {
		_increase_ref_count();
	}

	pseudo_static_pointer(pseudo_static_pointer&&) {
		_increase_ref_count();
	}

	pseudo_static_pointer& operator=(const pseudo_static_pointer&) noexcept = default;
	pseudo_static_pointer& operator=(pseudo_static_pointer&&) noexcept = default;

	      reference value()       { return *_value; }
	const_reference value() const { return *_value; }

	      reference operator*()       { return *_value; }
	const_reference operator*() const { return *_value; }

	      pointer operator->()       { return _value; }
	const_pointer operator->() const { return _value; }

private:
	static void _increase_ref_count() {
		_reference_count++;

		if (_reference_count == 1) {
			_value = new T;
		}
	}

	static void _decrease_ref_count() {
		_reference_count--;

		if (_reference_count == 0) {
			delete _value;
		}
	}

	static inline size_t _reference_count = 0;
	static inline pointer _value = nullptr;
};

}

#endif
