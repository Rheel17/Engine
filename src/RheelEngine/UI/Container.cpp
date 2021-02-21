/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "Container.h"

#include <iostream>

#include "UI.h"

namespace rheel {

std::optional<const Container::ConstraintTreeNode*> Container::ConstraintTreeNode::GetNodeForAnchor(const Constraint::Anchor& anchr) const {
	if (anchor == anchr) {
		return this;
	}

	for (auto child : children) {
		auto child_result = child.first->GetNodeForAnchor(anchr);
		if (child_result) {
			return child_result;
		}
	}

	return {};
}

std::optional<Container::ConstraintTreeNode*> Container::ConstraintTreeNode::GetNodeForAnchor(const Constraint::Anchor& anchr) {
	if (anchor == anchr) {
		return this;
	}

	for (auto child : children) {
		auto child_result = child.first->GetNodeForAnchor(anchr);
		if (child_result) {
			return child_result;
		}
	}

	return {};
}

Container::ConstraintTreeNode* Container::ConstraintTreeNode::NewRoot() {
	return new ConstraintTreeNode{ { nullptr, (Constraint::ConstraintLocation) -1 }, nullptr, {} };
}

Container::temporary_bounds::operator Element::Bounds() const {
	return { (unsigned) left, (unsigned) top, (unsigned) (right - left), (unsigned) (bottom - top) };
}

Container::Container() :
		Container(nullptr) {}

Container::Container(UI* ui) :
		_constraint_tree(ConstraintTreeNode::NewRoot()),
		_parent_ui(ui) {}

Container::Container(Container&& container) noexcept:
		Container() {

	*this = std::move(container);
}

Container::~Container() {
	_delete_constraint_tree(_constraint_tree);

	for (Element* element : _elements) {
		delete element;
	}
}

Container& Container::operator=(Container&& container) noexcept {
	// explicit self-move: return normally
	if (this == &container) {
		return *this;
	}

	// delete current elements and their constraints
	_delete_constraint_tree(_constraint_tree);

	for (Element* element : _elements) {
		delete element;
	}

	// move the elements
	_elements = std::move(container._elements);

	// set the correct parent of the elements
	for (Element* element : _elements) {
		element->_parent_container = this;
	}

	// move the constraint tree
	_constraint_tree = container._constraint_tree;
	container._constraint_tree = nullptr;

	// perform the Element moving
	MoveSuperFields(std::forward<Container>(container));
	_parent_ui = container._parent_ui;

	// finish
	return *this;
}

UI* Container::ParentUI() const {
	return _parent_ui;
}

void Container::RemoveElement(Element* element) {
	auto iter = std::find(_elements.begin(), _elements.end(), element);

	if (iter != _elements.end()) {
		_elements.erase(iter);
		delete element;
	}
}

Element* Container::ElementAt(unsigned x, unsigned y) {
	for (auto iter = _elements.rbegin(); iter != _elements.rend(); iter++) {
		Element* element = *iter;
		const Element::Bounds& bounds = element->GetBounds();

		// check if the coordinates are in the bounds of the element
		if (bounds.x <= x && x < bounds.x + bounds.width &&
				bounds.y <= y && y < bounds.y + bounds.height) {

			// nested resolve
			if (auto container = dynamic_cast<Container*>(element)) {
				return container->ElementAt(x, y);
			}

			// not a container: return the element
			return element;
		}
	}

	return this;
}

Element* Container::OpaqueElementAt(unsigned x, unsigned y) {
	for (auto iter = _elements.rbegin(); iter != _elements.rend(); iter++) {
		Element* element = *iter;
		const Element::Bounds& bounds = element->GetBounds();

		// check if the coordinates are in the bounds of the element, and that
		// the element is opaque
		if (bounds.x <= x && x < bounds.x + bounds.width &&
				bounds.y <= y && y < bounds.y + bounds.height &&
				element->IsOpaque()) {

			// nested resolve
			if (auto container = dynamic_cast<Container*>(element)) {
				Element* result = container->OpaqueElementAt(x, y);

				if (result == nullptr) {
					continue;
				}

				return result;
			}

			// not a container: return the element
			return element;
		}
	}

	return nullptr;
}

void Container::AddConstraint(Element* moving_element,
		Constraint::ConstraintLocation moving_location,
		Element* fixed_element,
		Constraint::ConstraintLocation fixed_location,
		int distance) {

	AddConstraint(Constraint(moving_element, moving_location, fixed_element, fixed_location, distance));
}

void Container::AddWidthRelativeConstraint(Element* moving_element,
		Constraint::ConstraintLocation moving_location,
		Element* fixed_element,
		Constraint::ConstraintLocation fixed_location,
		float distance) {

	AddConstraint(Constraint(moving_element, moving_location, fixed_element, fixed_location, Constraint::width_relative{ distance }));
}

void Container::AddHeightRelativeConstraint(Element* moving_element,
		Constraint::ConstraintLocation moving_location,
		Element* fixed_element,
		Constraint::ConstraintLocation fixed_location,
		float distance) {

	AddConstraint(Constraint(moving_element, moving_location, fixed_element, fixed_location, Constraint::height_relative{ distance }));
}

void Container::AddConstraint(const Constraint& constraint) {
	// check that the constraint is valid
	if (!constraint.IsValid()) {
		throw ConstraintException("Invalid constraint");
	}

	// check that the acting elements are not null and part of this container.
	_check_element(constraint.MovingAnchor().Element(), "Moving");
	_check_element(constraint.FixedAnchor().Element(), "Fixed");

	// check that the two acting elements are not the same
	if (constraint.FixedAnchor() == constraint.MovingAnchor()) {
		throw ConstraintException("Constraint must act on two different anchors");
	}

	// get the component constraints
	auto horizontal_constraint = constraint.HorizontalConstraint();
	auto vertical_constraint = constraint.VerticalConstraint();

	std::vector<Constraint> constraints;
	if (horizontal_constraint) {
		constraints.push_back(*horizontal_constraint);
	}
	if (vertical_constraint) {
		constraints.push_back(*vertical_constraint);
	}

	// check that the destination anchor is not already constrained
	std::optional<const Container::ConstraintTreeNode*> node;

	if (std::any_of(constraints.begin(), constraints.end(), [this, &node](const Constraint& c) -> bool {
		node = _constraint_tree->GetNodeForAnchor(c.MovingAnchor());

		// all destination anchors should either not exist, or not be
		// dependent on anything
		return node && (*node)->parent != nullptr && (*node)->parent != _constraint_tree;
	})) {
		std::ostringstream ss;
		ss << "The moving anchor is already constrained:\n" << constraint << "\n";
		ss << "Conflicting node: " << (*node)->anchor.Element() << "@" << (*node)->anchor.Location();

		throw ConstraintException(ss.str());
	}

	// actually add the component constraints.
	for (const Constraint& c : constraints) {

		// get or create the source node
		auto fixed_node_opt = _constraint_tree->GetNodeForAnchor(c.FixedAnchor());
		ConstraintTreeNode* fixed_node;

		if (fixed_node_opt) {
			fixed_node = *fixed_node_opt;
		} else {
			fixed_node = new ConstraintTreeNode{ c.FixedAnchor(), _constraint_tree, {} };
			_constraint_tree->children.insert({ fixed_node, c });
		}

		// if the destination node exists, delete it from its parent,
		// otherwise, create a new node
		auto moving_node_opt = _constraint_tree->GetNodeForAnchor(c.MovingAnchor());
		ConstraintTreeNode* moving_node;

		if (moving_node_opt) {
			moving_node = *moving_node_opt;
			ConstraintTreeNode* parent = moving_node->parent;
			parent->children.erase(*moving_node_opt);
		} else {
			moving_node = new ConstraintTreeNode{ c.MovingAnchor(), nullptr, {} };
		}

		// link the source and destination node
		moving_node->parent = fixed_node;
		fixed_node->children.insert({ moving_node, c });
	}
}

void Container::ClearConstraints() {
	_delete_constraint_tree(_constraint_tree);
	_constraint_tree = ConstraintTreeNode::NewRoot();
}

void Container::Layout() {
	// 0 elements dont't need to be constrained.
	if (_elements.empty()) {
		return;
	}

	// create a temporary map with bounds.
	TempBoundsMap bounds_map;

	// initialize the temporary map with the default bounds of each element
	for (Element* element : _elements) {
		element->InitializeBounds();

		bounds_map[element] = { 0, 0, 0, 0, false, false, false, false };
		bounds_map[element].left = GetBounds().x + element->GetBounds().x;
		bounds_map[element].top = GetBounds().y + element->GetBounds().y;
		bounds_map[element].right = GetBounds().x + element->GetBounds().x + element->GetBounds().width;
		bounds_map[element].bottom = GetBounds().y + element->GetBounds().y + element->GetBounds().height;
	}

	// initialize the self bounds with the container bounds
	bounds_map[nullptr] = {
			int(GetBounds().x),
			int(GetBounds().x + GetBounds().width),
			int(GetBounds().y),
			int(GetBounds().y + GetBounds().height),
			true, true, true, true
	};

	// layout the container
	_layout_node(bounds_map, _constraint_tree);

	// apply the bounds
	for (Element* element : _elements) {
		element->SetBounds(bounds_map[element]);
	}
}

void Container::DoDraw(float time, float dt) const {
	for (auto elem : _elements) {
		gl::ContextScope cs;
		elem->Draw(time, dt);
	}
}

void Container::OnResize() {
	Layout();
}

void Container::_check_element(Element* element, std::string source_or_destination) const {
	if (element == nullptr) {
		return;
	}

	// check that the element is in the element list.
	if (std::find(_elements.begin(), _elements.end(), element) == _elements.end()) {
		throw ConstraintException(source_or_destination + " element not in container");
	}
}

void Container::_delete_constraint_tree(ConstraintTreeNode* node) {
	if (!node) {
		return;
	}

	for (auto child : node->children) {
		_delete_constraint_tree(child.first);
	}

	delete node;
}

void Container::_layout_node(TempBoundsMap& bounds_map, ConstraintTreeNode* node) {
	for (auto child : node->children) {
		if (child.first->anchor.Element() != nullptr) {
			// actually apply the constraint
			_layout_node(bounds_map, child.second);
		}

		// recurse
		_layout_node(bounds_map, child.first);
	}
}

void Container::_layout_node(TempBoundsMap& bounds_map, const Constraint& constraint) {
	// we don't need to layout our own element
	if (constraint.MovingAnchor().Element() == nullptr) {
		return;
	}

	// extract the relevant elements
	auto elem_f = constraint.FixedAnchor().Element();
	auto elem_m = constraint.MovingAnchor().Element();

	// lambda for moving an anchor towards another anchor
	auto move = [](int fixed, int* moving, int* opposite, bool opposite_fixed, int distance) {
		int target = fixed + distance;
		int difference = target - *moving;

		*moving += difference;

		if (!opposite_fixed) {
			*opposite += difference;
		}
	};

	// lambda for checking that the invarient left < right is maintained
	auto check = [](const int& left, const int& right) {
		if (left > right) {
			throw ConstraintException("Applying constraint flipped the element");
		}
	};

	// switch for the different combinations, and apply the constraint
	int fixed;
	int* moving;
	int* opposite;
	bool opposite_fixed;
	int distance = constraint.Distance(GetBounds().width, GetBounds().height);

	switch (constraint.FixedAnchor().Location()) {
		case Constraint::EAST:
			fixed = bounds_map[elem_f].right;
			break;
		case Constraint::WEST:
			fixed = bounds_map[elem_f].left;
			break;
		case Constraint::NORTH:
			fixed = bounds_map[elem_f].top;
			break;
		case Constraint::SOUTH:
			fixed = bounds_map[elem_f].bottom;
			break;
		default:
			abort();
	}

	switch (constraint.MovingAnchor().Location()) {
		case Constraint::EAST:
			moving = &bounds_map[elem_m].right;
			opposite = &bounds_map[elem_m].left;
			opposite_fixed = bounds_map[elem_m].fixed_left;
			bounds_map[elem_m].fixed_right = true;
			distance *= -1;
			break;
		case Constraint::WEST:
			moving = &bounds_map[elem_m].left;
			opposite = &bounds_map[elem_m].right;
			opposite_fixed = bounds_map[elem_m].fixed_right;
			bounds_map[elem_m].fixed_left = true;
			break;
		case Constraint::NORTH:
			moving = &bounds_map[elem_m].top;
			opposite = &bounds_map[elem_m].bottom;
			opposite_fixed = bounds_map[elem_m].fixed_bottom;
			bounds_map[elem_m].fixed_top = true;
			break;
		case Constraint::SOUTH:
			moving = &bounds_map[elem_m].bottom;
			opposite = &bounds_map[elem_m].top;
			opposite_fixed = bounds_map[elem_m].fixed_top;
			bounds_map[elem_m].fixed_bottom = true;
			distance *= -1;
			break;
		default:
			abort();
	}

	move(fixed, moving, opposite, opposite_fixed, distance);
	check(bounds_map[elem_m].left, bounds_map[elem_m].right);
	check(bounds_map[elem_m].top, bounds_map[elem_m].bottom);
}

}
