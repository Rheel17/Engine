/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_ENTITY_H
#define RHEELENGINE_ENTITY_H
#include "../_common.h"

#include <atomic>

namespace rheel {

// Bit divisions for different entity ids. These divisions exist for the sole
// purpose of avoiding id-clashes. The first nibble is used to differentiate
// them.
//
// For the automatically generated ids: as it is (currently) impossible to have
// more than 2^63 entities in total over the whole lifetime of any game, we can
// simply increment from the last id, and return that. If 1000000 entity ids are
// generated every millisecond, it would take 292 years of continuous running
// for this generation scheme to be a problem. So don't do that.
//
// 0x00000000'00000000 for the null entity
// 0x01000000'00000000 for the root entity
// 0x02000000'xxxxxxxx for user-specified string ids (x = string hash)
// 0x03000000'xxxxxxxx for user-specified integral ids (x = id)
// 0x1xxxxxxx'xxxxxxxx for automatically generated ids (x = id)
class EntityId {
	friend class Registry;

public:
	/// This constructor allows the user to give an entity a name using a string
	/// literal. E.g. AddEntity("name"_id).
	constexpr EntityId(std::string_view sv) noexcept: // NOLINT (implicit)
			_value(_hash(sv) | 0x02000000'00000000ull) {}

	/// This construct allows the user to give an entity an id using a number
	/// value.
	constexpr EntityId(std::uint32_t value) noexcept: // NOLINT (implicit)
			_value(value | 0x03000000'00000000ull) {}

	/// Default constructor: the null entity
	constexpr EntityId() noexcept = default;

	constexpr static EntityId Root() noexcept {
		return EntityId(0x01000000'00000000ull);
	}

	constexpr auto operator<=>(const EntityId&) const noexcept = default;

private:
	constexpr explicit EntityId(std::uint64_t value) noexcept:
			_value(value) {}

	constexpr std::uint64_t _get_value() const noexcept {
		return _value;
	}

	constexpr static std::uint32_t _hash(std::string_view sv) noexcept {
		// 32-bit FNV-1a hash
		std::uint32_t val{ 0x811c9dc5u };

		for (char c : sv) {
			val ^= *reinterpret_cast<std::uint8_t*>(&c); // NOLINT (needed for proper conversion)
			val *= 0x01000193u;
		}

		return val;
	}

	static inline std::atomic_uint64_t _next_generated_id = 0x10000000'00000000ull;
	static EntityId _generate() {
		return _next_generated_id++;
	}

	std::uint64_t _value{}; // default-initialized with 0, which is equal to the null entity.
};

constexpr EntityId operator "" _id(const char* str, std::size_t n) noexcept {
	return EntityId(std::string_view(str, str + n));
}

class ConstEntityId {
	friend class Registry;

public:
	ConstEntityId(EntityId id) : // NOLINT (implicit)
			_id(id) {}

private:
	EntityId _id;

};

}

#endif
