/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Constraint.h"

namespace rheel {

Constraint::Anchor::Anchor() :
		_element(nullptr) {}

Constraint::Anchor::Anchor(rheel::Element* element, ConstraintLocation location) :
		_element(element),
		_location(location) {}

Element* Constraint::Anchor::Element() const {
	return _element;
}

Constraint::ConstraintLocation Constraint::Anchor::Location() const {
	return _location;
}

bool Constraint::Anchor::operator==(const Anchor& other) const {
	return other._element == _element && other._location == _location;
}

std::optional<Constraint::Anchor> Constraint::Anchor::HorizontalComponent() const {
	switch (_location) {
		case NORTH_EAST:
		case EAST:
		case SOUTH_EAST:
			return std::optional<Anchor>(Anchor(_element, EAST));
		case NORTH_WEST:
		case WEST:
		case SOUTH_WEST:
			return std::optional<Anchor>(Anchor(_element, WEST));
		default:
			return {};
	}
}

std::optional<Constraint::Anchor> Constraint::Anchor::VerticalComponent() const {
	switch (_location) {
		case NORTH_EAST:
		case NORTH:
		case NORTH_WEST:
			return std::optional<Anchor>(Anchor{ _element, NORTH });
		case SOUTH_EAST:
		case SOUTH:
		case SOUTH_WEST:
			return std::optional<Anchor>(Anchor{ _element, SOUTH });
		default:
			return {};
	}
}

Constraint::Constraint(Element* moving_element, ConstraintLocation moving_location, Element* fixed_element, ConstraintLocation fixed_location, int distance) :
		_moving(moving_element, moving_location),
		_fixed(fixed_element, fixed_location),
		_distance(distance),
		_distance_type(ABSOLUTE) {}

Constraint::Constraint(Element* moving_element,
		ConstraintLocation moving_location,
		Element* fixed_element,
		ConstraintLocation fixed_location,
		width_relative distance) :

		_moving(moving_element, moving_location),
		_fixed(fixed_element, fixed_location),
		_distance(distance.value),
		_distance_type(RELATIVE_TO_WIDTH) {}

Constraint::Constraint(Element* moving_element,
		ConstraintLocation moving_location,
		Element* fixed_element,
		ConstraintLocation fixed_location,
		height_relative distance) :
		_moving(moving_element, moving_location),
		_fixed(fixed_element, fixed_location),
		_distance(distance.value),
		_distance_type(RELATIVE_TO_HEIGHT) {}

Constraint Constraint::WithAnchors(const Anchor& moving, const Anchor& fixed) const {
	return Constraint(moving, fixed, _distance, _distance_type);
}

const Constraint::Anchor& Constraint::MovingAnchor() const {
	return _moving;
}

const Constraint::Anchor& Constraint::FixedAnchor() const {
	return _fixed;
}

int Constraint::Distance(unsigned width, unsigned height) const {
	switch (_distance_type) {
		case ABSOLUTE:
			return get<int>(_distance);
		case RELATIVE_TO_WIDTH:
			return width * get<float>(_distance);
		case RELATIVE_TO_HEIGHT:
			return height * get<float>(_distance);
		default:
			return 0;
	}
}

bool Constraint::IsDistanceRelative() const {
	return _distance_type != ABSOLUTE;
}

bool Constraint::operator==(const Constraint& other) const {
	return other._fixed == _fixed && other._moving == _moving && other._distance == _distance;
}

Constraint::Constraint(const Anchor& moving, const Anchor& fixed, distance_union distance, DistanceType distance_type) :
		_moving(moving),
		_fixed(fixed),
		_distance(distance),
		_distance_type(distance_type) {}

bool Constraint::IsValid() const {
	return static_cast<bool>(_fixed.HorizontalComponent()) == static_cast<bool>(_moving.HorizontalComponent()) &&
			static_cast<bool>(_fixed.VerticalComponent()) == static_cast<bool>(_moving.VerticalComponent());
}

std::optional<Constraint> Constraint::HorizontalConstraint() const {
	auto horizontal_moving = _moving.HorizontalComponent();
	auto horizontal_fixed = _fixed.HorizontalComponent();

	if (horizontal_moving && horizontal_fixed) {
		return Constraint(*horizontal_moving, *horizontal_fixed, _distance, _distance_type);
	}

	return {};
}

std::optional<Constraint> Constraint::VerticalConstraint() const {
	auto vertical_moving = _moving.VerticalComponent();
	auto vertical_fixed = _fixed.VerticalComponent();

	if (vertical_moving && vertical_fixed) {
		return Constraint(*vertical_moving, *vertical_fixed, _distance, _distance_type);
	}

	return {};
}

}

std::ostream& operator<<(std::ostream& stream, const rheel::Constraint& constraint) {
	static std::array<std::string_view, 8> location_strings = {
			"NORTH_WEST", "NORTH", "NORTH_EAST", "WEST", "EAST", "SOUTH_WEST", "SOUTH", "SOUTH_EAST"
	};

	return stream
			<< "Constraint{ Fixed=["
			<< constraint.FixedAnchor().Element()
			<< ", "
			<< location_strings[constraint.FixedAnchor().Location()]
			<< "], Moving=["
			<< constraint.MovingAnchor().Element()
			<< ", "
			<< location_strings[constraint.MovingAnchor().Location()]
			<< "], Distance="
			<< constraint.Distance()
			<< " }";
}
