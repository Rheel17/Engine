#ifndef CONTAINER_H_
#define CONTAINER_H_
#include "../../_common.h"

#include <vector>
#include <map>
#include <optional>

#include "Constraint.h"

namespace rheel {

class RE_API Container : public Element {
	friend class UI;

public:
	// copy constructor
	Container(const Container&);

	// copy assignment
	Container& operator=(const Container&);

	virtual ~Container();

	/**
	 * Adds an element to this container. Adding an element more than
	 * once is a no-op.
	 */
	void AddElement(ElementPtr element);

	/**
	 * Removes an element from this container. Removing an element not
	 * in this container is a no-op.
	 */
	void RemoveElement(ElementPtr element);

	/**
	 * Adds a constraint between elements in this container. For more
	 * details about constraints, look at the documentation of the
	 * Constraint class.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is
	 * invalid, the destination anchor is already constraint, the
	 * constraint is a self-loop, or if the elements are not in this
	 * container.
	 */
	void AddConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
			ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, int distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more
	 * details about constraints, look at the documentation of the
	 * Constraint class.
	 *
	 * The distance specified is multiplied by the container width to
	 * get the final distance.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is
	 * invalid, the destination anchor is already constraint, the
	 * constraint is a self-loop, or if the elements are not in this
	 * container.
	 */
	void AddWidthRelativeConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
			ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, float distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more
	 * details about constraints, look at the documentation of the
	 * Constraint class.
	 *
	 * The distance specified is multiplied by the container height to
	 * get the final distance.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is
	 * invalid, the destination anchor is already constraint, the
	 * constraint is a self-loop, or if the elements are not in this
	 * container.
	 */
	void AddHeightRelativeConstraint(ElementPtr movingElement, Constraint::ConstraintLocation movingLocation,
			ElementPtr fixedElement, Constraint::ConstraintLocation fixedLocation, float distance = 0);

	/**
	 * Adds a constraint between elements in this container. For more
	 * details about constraints, look at the documentation of the
	 * Constraint class.
	 *
	 * Can throw a rheel::ConstraintException if the constraint is
	 * invalid, the destination anchor is already constraint, the
	 * constraint is a self-loop, or if the elements are not in this
	 * container.
	 */
	void AddConstraint(const Constraint& constraint);

	/**
	 * Removes all constraints from this container.
	 */
	void ClearConstraints();

	/**
	 * Applies all constraints. The given width and height are the
	 * target dimensions of this container. If an element is constraint
	 * with the container itself, these values are used to resolve
	 * those constraints.
	 */
	void Layout(unsigned containerWidth, unsigned containerHeight);

	std::pair<unsigned, unsigned> GetDefaultDimensions() const {
		return std::make_pair(0, 0);
	}

	inline static std::shared_ptr<Container> Create() {
		return std::make_shared<Container>(Container());
	}

	/**
	 * Draws the container.
	 */
	void Draw() const override;

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

		inline operator Element::Bounds() {
			return { (unsigned) left, (unsigned) top, (unsigned) (right - left), (unsigned) (bottom - top) };
		}
	};

	using TempBoundsMap = std::map<ElementPtr, TemporaryBounds>;

	Container();

	void _CheckElement(ElementPtr element, std::string sourceOrDestination) const;
	void _DeleteConstraintTree(ConstraintTreeNode *node);

	static inline ConstraintTreeNode *_CreateEmptyConstraintTree() {
		return new ConstraintTreeNode { { nullptr, (Constraint::ConstraintLocation) -1 }, nullptr, {} };
	}

	void _LayoutNode(TempBoundsMap& boundsMap, ConstraintTreeNode *node, unsigned width, unsigned height);
	void _LayoutNode(TempBoundsMap& boundsMap, const Constraint& constraint, unsigned width, unsigned height);

	std::vector<ElementPtr> _elements;
	ConstraintTreeNode *_constraint_tree;

};

}

#endif
