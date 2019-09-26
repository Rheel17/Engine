/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include "Container.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "UI.h"

namespace rheel {

std::optional<const Container::ConstraintTreeNode *> Container::ConstraintTreeNode::GetNodeForAnchor(const Constraint::Anchor& anchor) const {
	if (this->anchor == anchor) {
		return this;
	}

	for (auto child : children) {
		auto childResult = child.first->GetNodeForAnchor(anchor);
		if (childResult) {
			return childResult;
		}
	}

	return {};
}

std::optional<Container::ConstraintTreeNode *> Container::ConstraintTreeNode::GetNodeForAnchor(const Constraint::Anchor& anchor) {
	if (this->anchor == anchor) {
		return this;
	}

	for (auto child : children) {
		auto childResult = child.first->GetNodeForAnchor(anchor);
		if (childResult) {
			return childResult;
		}
	}

	return {};
}

Container::ConstraintTreeNode *Container::ConstraintTreeNode::NewRoot() {
	return new ConstraintTreeNode { { nullptr, (Constraint::ConstraintLocation) -1 }, nullptr, {} };
}

Container::TemporaryBounds::operator Element::Bounds() const  {
	return { (unsigned) left, (unsigned) top, (unsigned) (right - left), (unsigned) (bottom - top) };
}

Container::Container() :
		Container(nullptr) {}

Container::Container(UI *ui) :
		_constraint_tree(ConstraintTreeNode::NewRoot()),
		_parent_ui(ui) {}

Container::Container(Container&& container) :
		Container() {

	*this = std::move(container);
}

Container::~Container() {
	_DeleteConstraintTree(_constraint_tree);

	for (Element *element : _elements) {
		delete element;
	}
}

Container& Container::operator=(Container&& container) {
	// explicit self-move: return normally
	if (this == &container) {
		return *this;
	}

	// perform the Element moving
	_MoveSuperFields(std::move(container));
	_parent_ui = container._parent_ui;

	// delete current elements and their constraints
	_DeleteConstraintTree(_constraint_tree);

	for (Element *element : _elements) {
		delete element;
	}

	// move the elements
	_elements = std::move(container._elements);

	// set the correct parent of the elements
	for (Element *element : _elements) {
		element->_parent_container = this;
	}

	// move the constraint tree
	_constraint_tree = std::move(container._constraint_tree);
	container._constraint_tree = nullptr;

	// finish
	return *this;
}

UI *Container::ParentUI() const {
	return _parent_ui;
}

void Container::RemoveElement(Element *element) {
	auto iter = std::find(_elements.begin(), _elements.end(), element);

	if (iter != _elements.end()) {
		_elements.erase(iter);
		delete element;
	}
}

Element *Container::ElementAt(unsigned x, unsigned y) {
	for (auto iter = _elements.rbegin(); iter != _elements.rend(); iter++) {
		Element *element = *iter;
		const Element::Bounds& bounds = element->GetBounds();

		// check if the coordinates are in the bounds of the element
		if (bounds.x <= x && x < bounds.x + bounds.width &&
				bounds.y <= y && y < bounds.y + bounds.height) {

			// nested resolve
			if (Container *container = dynamic_cast<Container *>(element)) {
				return container->ElementAt(x, y);
			}

			// not a container: return the element
			return element;
		}
	}

	return this;
}

Element *Container::OpaqueElementAt(unsigned x, unsigned y) {
	for (auto iter = _elements.rbegin(); iter != _elements.rend(); iter++) {
		Element *element = *iter;
		const Element::Bounds& bounds = element->GetBounds();

		// check if the coordinates are in the bounds of the element, and that
		// the element is opaque
		if (bounds.x <= x && x < bounds.x + bounds.width &&
				bounds.y <= y && y < bounds.y + bounds.height &&
				element->IsOpaque()) {

			// nested resolve
			if (Container *container = dynamic_cast<Container *>(element)) {
				Element *result = container->OpaqueElementAt(x, y);

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

void Container::AddConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
		Element *fixedElement, Constraint::ConstraintLocation fixedLocation, int distance) {
	AddConstraint(Constraint(movingElement, movingLocation, fixedElement, fixedLocation, distance));
}

void Container::AddWidthRelativeConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
		Element *fixedElement, Constraint::ConstraintLocation fixedLocation, float distance) {
	AddConstraint(Constraint(movingElement, movingLocation, fixedElement, fixedLocation, Constraint::WidthRelative { distance }));
}

void Container::AddHeightRelativeConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
		Element *fixedElement, Constraint::ConstraintLocation fixedLocation, float distance) {
	AddConstraint(Constraint(movingElement, movingLocation, fixedElement, fixedLocation, Constraint::HeightRelative { distance }));
}

void Container::AddConstraint(const Constraint& constraint) {
	// check that the constraint is valid
	if (!constraint.IsValid()) {
		throw ConstraintException("Invalid constraint");
	}

	// check that the acting elements are not null and part of this container.
	_CheckElement(constraint.MovingAnchor().Element(), "Moving");
	_CheckElement(constraint.FixedAnchor().Element(), "Fixed");

	// check that the two acting elements are not the same
	if (constraint.FixedAnchor() == constraint.MovingAnchor()) {
		throw ConstraintException("Constraint must act on two different anchors");
	}

	// get the component constraints
	auto horizontalConstraint = constraint.HorizontalConstraint();
	auto verticalConstraint = constraint.VerticalConstraint();

	std::vector<Constraint> constraints;
	if (horizontalConstraint) constraints.push_back(*horizontalConstraint);
	if (verticalConstraint) constraints.push_back(*verticalConstraint);

	// check that the destination anchor is not already constrained
	std::optional<const Container::ConstraintTreeNode *> node;

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
		auto fixedNodeOpt = _constraint_tree->GetNodeForAnchor(c.FixedAnchor());
		ConstraintTreeNode *fixedNode;

		if (fixedNodeOpt) {
			fixedNode = *fixedNodeOpt;
		} else {
			fixedNode = new ConstraintTreeNode { c.FixedAnchor(), _constraint_tree, {} };
			_constraint_tree->children.insert({ fixedNode, c });
		}

		// if the destination node exists, delete it from its parent,
		// otherwise, create a new node
		auto movingNodeOpt = _constraint_tree->GetNodeForAnchor(c.MovingAnchor());
		ConstraintTreeNode *movingNode;

		if (movingNodeOpt) {
			movingNode = *movingNodeOpt;
			ConstraintTreeNode *parent = movingNode->parent;
			parent->children.erase(*movingNodeOpt);
		} else {
			movingNode = new ConstraintTreeNode { c.MovingAnchor(), nullptr, {} };
		}

		// link the source and destination node
		movingNode->parent = fixedNode;
		fixedNode->children.insert({ movingNode, c });
	}
}

void Container::ClearConstraints() {
	_DeleteConstraintTree(_constraint_tree);
	_constraint_tree = ConstraintTreeNode::NewRoot();
}

void Container::Layout() {
	// 0 elements dont't need to be constrained.
	if (_elements.empty()) {
		return;
	}

	// create a temporary map with bounds.
	TempBoundsMap boundsMap;

	// initialize the temporary map with the default bounds of each element
	for (Element *element : _elements) {
		element->InitializeBounds();

		boundsMap[element] = { 0, 0, 0, 0, false, false, false, false };
		boundsMap[element].left = GetBounds().x + element->GetBounds().x;
		boundsMap[element].top = GetBounds().y + element->GetBounds().y;
		boundsMap[element].right = GetBounds().x + element->GetBounds().x + element->GetBounds().width;
		boundsMap[element].bottom = GetBounds().y + element->GetBounds().y + element->GetBounds().height;
	}

	// initialize the self bounds with the container bounds
	boundsMap[nullptr] = {
			int(GetBounds().x),
			int(GetBounds().x + GetBounds().width),
			int(GetBounds().y),
			int(GetBounds().y + GetBounds().height),
			true, true, true, true };

	// layout the container
	_LayoutNode(boundsMap, _constraint_tree);

	// apply the bounds
	for (Element *element : _elements) {
		element->SetBounds(boundsMap[element]);
	}
}

void Container::Draw(float dt) const {
	for (auto elem : _elements) {
		elem->Draw(dt);
	}
}

void Container::OnResize() {
	Layout();
}

void Container::_CheckElement(Element *element, std::string sourceOrDestination) const {
	if (element == nullptr) {
		return;
	}

	// check that the element is in the element list.
	if (std::find(_elements.begin(), _elements.end(), element) == _elements.end()) {
		throw ConstraintException(sourceOrDestination + " element not in container");
	}
}

void Container::_DeleteConstraintTree(ConstraintTreeNode *node) {
	if (!node) {
		return;
	}

	for (auto child : node->children) {
		_DeleteConstraintTree(child.first);
	}

	delete node;
}

void Container::_LayoutNode(TempBoundsMap& boundsMap, ConstraintTreeNode *node) {
	for (auto child : node->children) {
		if (child.first->anchor.Element() != nullptr) {
			// actually apply the constraint
			_LayoutNode(boundsMap, child.second);
		}

		// recurse
		_LayoutNode(boundsMap, child.first);
	}
}

void Container::_LayoutNode(TempBoundsMap& boundsMap, const Constraint& constraint) {
	// we don't need to layout our own element
	if (constraint.MovingAnchor().Element() == nullptr) {
		return;
	}

	// extract the relevant elements
	auto elem_f = constraint.FixedAnchor().Element();
	auto elem_m = constraint.MovingAnchor().Element();

	// lambda for moving an anchor towards another anchor
	auto move = [](int fixed, int *moving, int *opposite, bool oppositeFixed, int distance) {
		int target = fixed + distance;
		int difference = target - *moving;

		*moving += difference;

		if (!oppositeFixed) {
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
	int *moving;
	int *opposite;
	bool oppositeFixed;
	int distance = constraint.Distance(GetBounds().width, GetBounds().height);

	switch(constraint.FixedAnchor().Location()) {
		case Constraint::EAST:  fixed = boundsMap[elem_f].right;  break;
		case Constraint::WEST:  fixed = boundsMap[elem_f].left;   break;
		case Constraint::NORTH: fixed = boundsMap[elem_f].top;    break;
		case Constraint::SOUTH: fixed = boundsMap[elem_f].bottom; break;
		default: abort();
	}

	switch(constraint.MovingAnchor().Location()) {
		case Constraint::EAST:
			moving = &boundsMap[elem_m].right;
			opposite = &boundsMap[elem_m].left;
			oppositeFixed = boundsMap[elem_m].fixed_left;
			boundsMap[elem_m].fixed_right = true;
			distance *= -1;
			break;
		case Constraint::WEST:
			moving = &boundsMap[elem_m].left;
			opposite = &boundsMap[elem_m].right;
			oppositeFixed = boundsMap[elem_m].fixed_right;
			boundsMap[elem_m].fixed_left = true;
			break;
		case Constraint::NORTH:
			moving = &boundsMap[elem_m].top;
			opposite = &boundsMap[elem_m].bottom;
			oppositeFixed = boundsMap[elem_m].fixed_bottom;
			boundsMap[elem_m].fixed_top = true;
			break;
		case Constraint::SOUTH:
			moving = &boundsMap[elem_m].bottom;
			opposite = &boundsMap[elem_m].top;
			oppositeFixed = boundsMap[elem_m].fixed_top;
			boundsMap[elem_m].fixed_bottom = true;
			distance *= -1;
			break;
		default:
			abort();
	}

	move(fixed, moving, opposite, oppositeFixed, distance);
	check(boundsMap[elem_m].left, boundsMap[elem_m].right);
	check(boundsMap[elem_m].top, boundsMap[elem_m].bottom);
}

}
