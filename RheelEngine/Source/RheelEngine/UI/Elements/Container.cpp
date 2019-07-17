#include "Container.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace rheel {

Container::Container() :
		_constraint_tree(_CreateEmptyConstraintTree()) {}

Container::Container(const Container& container) :
		_constraint_tree(_CreateEmptyConstraintTree()) {

	*this = container;
}

Container::~Container() {
	_DeleteConstraintTree(_constraint_tree);
}

Container& Container::operator=(const Container& container) {
	// replace the element list
	_elements = container._elements;

	// replace the constraint tree
	_DeleteConstraintTree(_constraint_tree);
	_constraint_tree = container._constraint_tree->Copy(nullptr);

	// replace the _container_element references in the constraint tree to
	// the actual _container_element of this container.
	for (int loc = Constraint::LOCATION_ITERATOR_BEGIN; loc != Constraint::LOCATION_ITERATOR_END; loc++) {
		auto nodeOpt = _constraint_tree->GetNodeForAnchor(Constraint::Anchor {
			nullptr,
			static_cast<Constraint::ConstraintLocation>(loc)
		});

		if (nodeOpt) {
			(*nodeOpt)->anchor = Constraint::Anchor(nullptr, (*nodeOpt)->anchor.Location());
		}
	}

	return *this;
}

void Container::AddElement(ElementPtr element) {
	// no-op if the element is already in the container.
	if (std::find(_elements.begin(), _elements.end(), element) != _elements.end()) {
		return;
	}

	_elements.push_back(element);
}

void Container::RemoveElement(ElementPtr element) {
	_elements.erase(std::find(_elements.begin(), _elements.end(), element));
}

void Container::_CheckElement(ElementPtr element, std::string sourceOrDestination) const {
	if (element == nullptr) {
		return;
	}

	// check that the element is in the element list.
	if (std::find(_elements.begin(), _elements.end(), element) == _elements.end()) {
		throw ConstraintException(sourceOrDestination + " element not in container");
	}
}

void Container::AddConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
		ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, int distance) {
	AddConstraint(Constraint(movingElement, movingLocation, fixedElement, fixedLocation, distance));
}

void Container::AddWidthRelativeConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
		ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, float distance) {
	AddConstraint(Constraint(movingElement, movingLocation, fixedElement, fixedLocation, Constraint::WidthRelative { distance }));
}

void Container::AddHeightRelativeConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
		ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, float distance) {
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
	_constraint_tree = _CreateEmptyConstraintTree();
}

void Container::Layout(unsigned containerWidth, unsigned containerHeight) {
	// 0 elements dont't need to be constrained.
	if (_elements.empty()) {
		return;
	}

	// create a temporary map with bounds.
	TempBoundsMap boundsMap;

	// initialize the temporary map with the default bounds of each element
	for (ElementPtr element : _elements) {
		element->InitializeBounds();

		boundsMap[element] = { 0, 0, 0, 0, false, false, false, false };
		boundsMap[element].left = element->GetBounds().x;
		boundsMap[element].top = element->GetBounds().y;
		boundsMap[element].right = element->GetBounds().x + element->GetBounds().width;
		boundsMap[element].bottom = element->GetBounds().y + element->GetBounds().height;
	}

	// initialize the self bounds with the container bounds
	boundsMap[nullptr] = { 0, (int) containerWidth, 0, (int) containerHeight, true, true, true, true };

	// layout the container
	_LayoutNode(boundsMap, _constraint_tree, containerWidth, containerHeight);

	// apply the bounds
	for (ElementPtr element : _elements) {
		element->SetBounds(boundsMap[element]);
	}
}

void Container::_LayoutNode(TempBoundsMap& boundsMap, ConstraintTreeNode *node, unsigned width, unsigned height) {
	for (auto child : node->children) {
		if (child.first->anchor.Element() != nullptr) {
			// actually apply the constraint
			_LayoutNode(boundsMap, child.second, width, height);
		}

		// recurse
		_LayoutNode(boundsMap, child.first, width, height);
	}
}

void Container::_LayoutNode(TempBoundsMap& boundsMap, const Constraint& constraint, unsigned width, unsigned height) {
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
	int distance = constraint.Distance(width, height);

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
			break;
		default:
			abort();
	}

	move(fixed, moving, opposite, oppositeFixed, distance);
	check(boundsMap[elem_m].left, boundsMap[elem_m].right);
	check(boundsMap[elem_m].top, boundsMap[elem_m].bottom);
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

Container::ConstraintTreeNode *Container::ConstraintTreeNode::Copy(ConstraintTreeNode *parent) const {
	ConstraintTreeNode *copy = new ConstraintTreeNode;
	copy->anchor = anchor;
	copy->parent = parent;

	for (auto child : children) {
		copy->children.insert({ child.first->Copy(copy), child.second });
	}

	return copy;
}

}
