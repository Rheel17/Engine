/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef CONTAINER_H_
#define CONTAINER_H_
#include "../_common.h"

#include <map>
#include <optional>

#include "Constraint.h"

namespace rheel {

class UI;

class RE_API Container : public Element {
	friend class UI;

	RE_NO_COPY(Container)

private:
	class ConstraintTreeNode {

	public:
		std::optional<const ConstraintTreeNode *> GetNodeForAnchor(const Constraint::Anchor& anchor) const;
		std::optional<ConstraintTreeNode *> GetNodeForAnchor(const Constraint::Anchor& anchor);

		Constraint::Anchor anchor;
		ConstraintTreeNode *parent;
		std::map<ConstraintTreeNode *, Constraint> children;

	public:
		static ConstraintTreeNode *NewRoot();

	};

	struct TemporaryBounds {
		int left, right;
		int top, bottom;
		bool fixed_left, fixed_right;
		bool fixed_top, fixed_bottom;

		operator Element::Bounds() const;
	};

public:
	/**
	 * Constructs a default empty container
	 */
	Container();

	/**
	 * Moves the given container into a new container
	 */
	explicit Container(Container&& container) noexcept;

	~Container() override;

	/**
	 * Moves the given container into this container
	 */
	Container& operator=(Container&& container) noexcept;

	/**
	 * Returns the parent UI of this container.
	 */
	UI *ParentUI() const;

	/**
	 * Adds an element to this container. The element is copied into a pointer,
	 * which is returned by this method. Use this pointer to reference the
	 * element.
	 */
	template<typename T>
	T *AddElement(const T& element) {
		static_assert(std::is_base_of<Element, T>::value, "Element must derive from the Element class");
		static_assert(std::is_copy_constructible<T>::value, "Element must be copy-constructible; try InsertElement with std::move()");

		T *ptr = new T(element);
		ptr->_parent_container = this;
		_elements.push_back(ptr);

		return ptr;
	}

	/**
	 * Inserts the element to this container. The element is moved into a
	 * pointer, which is returned by this method. Use this pointer to reference
	 * the element. The element reference passed to this method will no longer
	 * be valid after this method returns.
	 */
	template<typename T>
	T *InsertElement(T&& element) {
		static_assert(std::is_base_of<Element, T>::value, "Element must derive from the Element class, did you std::move()?");
		static_assert(std::is_move_constructible<T>::value, "Element must be move-constructible");

		T *ptr = new T(std::forward<T>(element));
		ptr->_parent_container = this;
		_elements.push_back(ptr);

		return ptr;
	}

	/**
	 * Removes an element from this container. Removing an element not in this
	 * container has no effect.
	 */
	void RemoveElement(Element *element);

	/**
	 * Returns the element at the specified position. When no element has been
	 * added at the given position, this container is returned. If multiple
	 * elements share the position, the top one (last added) is returned.
	 */
	Element *ElementAt(unsigned x, unsigned y);

	/**
	 * Returns the element at the specified position. When no element has been
	 * added at the given position, this container is returned. If multiple
	 * elements share the position, the top opaque one (last added) is returned.
	 */
	Element *OpaqueElementAt(unsigned x, unsigned y);

	/**
	 * Adds a constraint between elements in this container. For more details
	 * about constraints, look at the documentation of the Constraint class.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is invalid, the
	 * destination anchor is already constraint, the constraint is a self-loop,
	 * or if the elements are not in this container.
	 */
	void AddConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
			Element *fixedElement, Constraint::ConstraintLocation fixedLocation, int distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more details
	 * about constraints, look at the documentation of the Constraint class.
	 *
	 * The distance specified is multiplied by the container width to get the
	 * final distance.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is invalid, the
	 * destination anchor is already constraint, the constraint is a self-loop,
	 * or if the elements are not in this container.
	 */
	void AddWidthRelativeConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
			Element *fixedElement, Constraint::ConstraintLocation fixedLocation, float distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more details
	 * about constraints, look at the documentation of the Constraint class.
	 *
	 * The distance specified is multiplied by the container height to get the
	 * final distance.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is invalid, the
	 * destination anchor is already constraint, the constraint is a self-loop,
	 * or if the elements are not in this container.
	 */
	void AddHeightRelativeConstraint(Element *movingElement, Constraint::ConstraintLocation movingLocation,
			Element *fixedElement, Constraint::ConstraintLocation fixedLocation, float distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more details
	 * about constraints, look at the documentation of the Constraint class.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is invalid, the
	 * destination anchor is already constraint, the constraint is a self-loop,
	 * or if the elements are not in this container.
	 */
	void AddConstraint(const Constraint& constraint);

	/**
	 * Removes all constraints from this container.
	 */
	void ClearConstraints();

	/**
	 * Applies all constraints and calculates the bounds of the child elements.
	 * This methods is automatically called when the container is resized.
	 */
	void Layout();

	/**
	 * Draws the container.
	 */
	void Draw(float dt) const override;

	/**
	 * Called when this element has been resized.
	 */
	void OnResize() override;

private:
	using TempBoundsMap = std::map<Element *, TemporaryBounds>;

	Container(UI *ui);

	void _CheckElement(Element *element, std::string sourceOrDestination) const;
	void _DeleteConstraintTree(ConstraintTreeNode *node);

	void _LayoutNode(TempBoundsMap& boundsMap, ConstraintTreeNode *node);
	void _LayoutNode(TempBoundsMap& boundsMap, const Constraint& constraint);

	std::vector<Element *> _elements;
	ConstraintTreeNode *_constraint_tree;
	UI *_parent_ui;

};

}

#endif
