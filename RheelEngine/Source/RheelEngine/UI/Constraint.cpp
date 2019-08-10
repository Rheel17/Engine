#include "Constraint.h"

namespace rheel {

Constraint::Anchor::Anchor() :
		_element(nullptr) {}

Constraint::Anchor::Anchor(rheel::Element *element, ConstraintLocation location) :
		_element(element), _location(location) {}

Element *Constraint::Anchor::Element() const {
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
			return Anchor(_element, EAST);
		case NORTH_WEST:
		case WEST:
		case SOUTH_WEST:
			return Anchor(_element, WEST);
		default:
			return {};
	}
}

std::optional<Constraint::Anchor> Constraint::Anchor::VerticalComponent() const {
	switch (_location) {
		case NORTH_EAST:
		case NORTH:
		case NORTH_WEST:
			return Anchor { _element, NORTH };
		case SOUTH_EAST:
		case SOUTH:
		case SOUTH_WEST:
			return Anchor { _element, SOUTH };
		default:
			return {};
	}
}

Constraint::Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, int distance) :
		_moving(movingElement, movingLocation), _fixed(fixedElement, fixedLocation), _distance_type(ABSOLUTE) {

	_distance.distance = distance;
}

Constraint::Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, WidthRelative distance) :
		_moving(movingElement, movingLocation), _fixed(fixedElement, fixedLocation), _distance_type(RELATIVE_TO_WIDTH) {

	_distance.distance_relative = distance.value;
}

Constraint::Constraint(Element *movingElement, ConstraintLocation movingLocation, Element *fixedElement, ConstraintLocation fixedLocation, HeightRelative distance) :
		_moving(movingElement, movingLocation), _fixed(fixedElement, fixedLocation), _distance_type(RELATIVE_TO_HEIGHT) {

	_distance.distance_relative = distance.value;
}

const Constraint::Anchor& Constraint::MovingAnchor() const {
	return _moving;
}

const Constraint::Anchor& Constraint::FixedAnchor() const {
	return _fixed;
}

int Constraint::Distance(unsigned width, unsigned height) const {
	switch (_distance_type) {
		case ABSOLUTE: return _distance.distance;
		case RELATIVE_TO_WIDTH: return width * _distance.distance_relative;
		case RELATIVE_TO_HEIGHT: return height * _distance.distance_relative;
		default: return 0;
	}
}

bool Constraint::IsDistanceRelative() const {
	return _distance_type != ABSOLUTE;
}

bool Constraint::operator==(const Constraint& other) const {
	return other._fixed == _fixed && other._moving == _moving && other._distance.distance == _distance.distance;
}

Constraint::Constraint(const Anchor& moving, const Anchor& fixed, _DistanceUnion distance, _DistanceType distanceType) :
		_moving(moving), _fixed(fixed), _distance(distance), _distance_type(distanceType) {}

bool Constraint::IsValid() const {
	return static_cast<bool>(_fixed.HorizontalComponent()) ==
					static_cast<bool>(_moving.HorizontalComponent()) &&
			static_cast<bool>(_fixed.VerticalComponent()) ==
					static_cast<bool>(_moving.VerticalComponent());
}

std::optional<Constraint> Constraint::HorizontalConstraint() const {
	auto horizontalMoving = _moving.HorizontalComponent();
	auto horizontalFixed = _fixed.HorizontalComponent();

	if (horizontalMoving && horizontalFixed) {
		return Constraint(*horizontalMoving, *horizontalFixed, _distance, _distance_type);
	}

	return {};
}

std::optional<Constraint> Constraint::VerticalConstraint() const {
	auto verticalMoving = _moving.VerticalComponent();
	auto verticalFixed = _fixed.VerticalComponent();

	if (verticalMoving && verticalFixed) {
		return Constraint(*verticalMoving, *verticalFixed, _distance, _distance_type);
	}

	return {};
}

}

std::ostream& operator<<(std::ostream& stream, const rheel::Constraint& constraint)  {
	static const char *location_strings[] = {
			"NORTH_WEST", "NORTH", "NORTH_EAST", "WEST", "EAST", "SOUTH_WEST", "SOUTH", "SOUTH_EAST"
	};

	return stream << "Constraint{ Fixed=[" <<
				constraint.FixedAnchor().Element() << ", " << location_strings[constraint.FixedAnchor().Location()] <<
				"], Moving=[" <<
				constraint.MovingAnchor().Element() << ", " << location_strings[constraint.MovingAnchor().Location()] <<
				"], Distance=" << constraint.Distance() << " }";
}


