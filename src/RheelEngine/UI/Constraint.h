/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_
#include "../_common.h"

#include "Element.h"

#include <exception>
#include <optional>

namespace rheel {

/**
 * A Constraint is used to constraint elements in a Container. When handling a
 * constraint, the layout engine will fix the 'source' element in place, and
 * move and/or resize the destination element until the constraint is satisfied.
 *
 * Unconstrained elements will sit at (0, 0) with their default size, or need to
 * have their bounds set manually.
 *
 * NOTE that not all constraints make sense, and before adding a constraint, the
 * container will check and throw an exception if the constraint doesn't make
 * sense.
 */
class RE_API Constraint {

public:
	/**
	 * A ConstraintLocation represents a location relative to a given element,
	 * with each symbol meaning the following:
	 *
	 * - NORTH_WEST: top left
	 * - NORTH:      top
	 * - NORTH_EAST: top right
	 * - WEST:       left
	 * - EAST:       right
	 * - SOUTH_WEST: bottom left
	 * - SOUTH:      bottom
	 * - SOUTH_EAST: bottom right
	 */
	enum ConstraintLocation {
		 NORTH_WEST ,    NORTH   , NORTH_EAST ,
		    WEST    ,                 EAST    ,
		 SOUTH_WEST ,    SOUTH   , SOUTH_EAST ,

		    TOP_LEFT = NORTH_WEST ,    TOP = NORTH ,    TOP_RIGHT = NORTH_EAST ,
		        LEFT = WEST       ,                         RIGHT = EAST       ,
		 BOTTOM_LEFT = SOUTH_WEST , BOTTOM = SOUTH , BOTTOM_RIGHT = SOUTH_EAST ,
	};

	struct WidthRelative  { float value; };
	struct HeightRelative { float value; };

	/**
	 * An Anchor represents a location on a certain element.
	 */
	class Anchor {

	public:
		Anchor();
		Anchor(rheel::Element *element, ConstraintLocation location);

		/**
		 * Returns the element of this anchor
		 */
		rheel::Element *Element() const;

		/**
		 * Returns the location on the element.
		 */
		ConstraintLocation Location() const;

		/**
		 * Returns the horizontal component of this anchor
		 */
		std::optional<Anchor> HorizontalComponent() const;

		/**
		 * Returns the vertical component of this anchor
		 */
		std::optional<Anchor> VerticalComponent() const;

		/**
		 * Compares this anchor with another one.
		 */
		bool operator==(const Anchor& other) const;

	private:
		rheel::Element *_element;
		ConstraintLocation _location = ConstraintLocation::NORTH_WEST;

	};

private:
	union _DistanceUnion {
		int distance;
		float distance_relative;
	};

	enum _DistanceType {
		ABSOLUTE, RELATIVE_TO_WIDTH, RELATIVE_TO_HEIGHT
	};

public:
	Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, int distance = 0);
	Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, WidthRelative distance);
	Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, HeightRelative distance);

	/**
	 * Copies this constraint, but replaces the anchors. The distance remains
	 * the same.
	 */
	Constraint WithAnchors(const Anchor& moving, const Anchor& fixed) const;

	/**
	 * Returns the moving anchor: the anchor that moves towards the fixed
	 * anchor.
	 */
	const Anchor& MovingAnchor() const;

	/**
	 * Returns the fixed anchor: the anchor that stays in place and dictates
	 * where the moving anchor goes.
	 */
	const Anchor& FixedAnchor() const;

	/**
	 * Returns the required distance between the two anchors.
	 */
	int Distance(unsigned width = 0, unsigned height = 0) const;

	/**
	 * Returns true if the distance specified is relative.
	 */
	bool IsDistanceRelative() const;

	/**
	 * Returns an optional constraint representing the horizontal component of
	 * this constraint.
	 */
	std::optional<Constraint> HorizontalConstraint() const;

	/**
	 * Returns an optional constraint representing the vertical component of
	 * this constraint.
	 */
	std::optional<Constraint> VerticalConstraint() const;

	/**
	 * Returns true is this is a valid constraint.
	 */
	bool IsValid() const;

	/**
	 * Compares this constraint with another one.
	 */
	bool operator==(const Constraint& other) const;

private:
	Constraint(const Anchor& moving, const Anchor& fixed, _DistanceUnion distance, _DistanceType distanceType);

	Anchor _moving;
	Anchor _fixed;
	_DistanceUnion _distance{};
	_DistanceType _distance_type;

public:
	static constexpr ConstraintLocation LOCATION_ITERATOR_BEGIN = NORTH_WEST;
	static constexpr ConstraintLocation LOCATION_ITERATOR_END = SOUTH_EAST;

	static constexpr bool IsLocationPure(ConstraintLocation location) {
		return location == NORTH || location == SOUTH || location == EAST || location == WEST;
	}
};

class RE_API ConstraintException : public std::exception {

public:
	explicit ConstraintException(std::string what) : _what(std::move(what)) {}
	const char *what() const noexcept override { return _what.c_str(); }

private:
	std::string _what;

};

}

std::ostream& operator<<(std::ostream& stream, const rheel::Constraint& constraint);

#endif
