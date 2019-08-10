#ifndef CONTAINER_H_
#define CONTAINER_H_
#include "../_common.h"

#include <vector>
#include <map>
#include <optional>

#include "Constraint.h"

namespace rheel {

class UI;

class RE_API Container : public Element {
	friend class UI;

	__ELEMENT__

	RE_NO_MOVE(Container);

private:
	struct ConstraintTreeNode {
		Constraint::Anchor anchor;
		ConstraintTreeNode *parent;
		std::map<ConstraintTreeNode *, Constraint> children;

		std::optional<const ConstraintTreeNode *> GetNodeForAnchor(const Constraint::Anchor& anchor) const;
		std::optional<ConstraintTreeNode *> GetNodeForAnchor(const Constraint::Anchor& anchor);

		ConstraintTreeNode *Copy(ConstraintTreeNode *parent) const;
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
	 * Copies the given container into a new container
	 */
	Container(const Container& container);

	~Container();

	/**
	 * Copies the given container into this container
	 */
	Container& operator=(const Container& container);

	/**
	 * Adds an element to this container. The element is copied into a pointer,
	 * which is returned by this method. Use this pointer to reference the
	 * element.
	 */
	template<typename T>
	Element *AddElement(const T& element) {
		static_assert(std::is_base_of<Element, T>::value, "Element must derive from the Element class");
		static_assert(std::is_copy_constructible<T>::value, "Element must be copy-constructible");

		Element *ptr = new T(element);
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
	 * Applies all constraints. The given width and height are the target
	 * dimensions of this container. If an element is constraint with the
	 * container itself, these values are used to resolve those constraints.
	 */
	void Layout(unsigned containerWidth, unsigned containerHeight);

	/**
	 * Draws the container.
	 */
	void Draw() const override;

private:
	using TempBoundsMap = std::map<Element *, TemporaryBounds>;

	void _CheckElement(Element *element, std::string sourceOrDestination) const;
	void _DeleteConstraintTree(ConstraintTreeNode *node);

	static inline ConstraintTreeNode *_CreateEmptyConstraintTree() {
		return new ConstraintTreeNode { { nullptr, (Constraint::ConstraintLocation) -1 }, nullptr, {} };
	}

	void _LayoutNode(TempBoundsMap& boundsMap, ConstraintTreeNode *node, unsigned width, unsigned height);
	void _LayoutNode(TempBoundsMap& boundsMap, const Constraint& constraint, unsigned width, unsigned height);

	std::vector<Element *> _elements;
	ConstraintTreeNode *_constraint_tree;
	UI *_parent_ui;

};

}

#endif
