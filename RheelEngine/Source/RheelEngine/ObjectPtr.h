#ifndef OBJECTPTR_H_
#define OBJECTPTR_H_
#include "_common.h"

#include <memory>

namespace rheel {

class Object;

/**
 * The ObjectPtr class provides a wrapper around an Object *. When the address
 * of an Object changes, the internals of this class will reflect those changes.
 *
 * Instances of this object can be safely copied and moved.
 */
class RE_API ObjectPtr {
	friend class Object;
	friend class Scene;

public:
	/**
	 * Constructs a default object pointer, pointing to no object (nullptr).
	 */
	ObjectPtr();

	/**
	 * Used to access Object members. This is the only valid use of this method.
	 *
	 * Under no circumstance can this method be used to extract the raw pointer.
	 * No guarantees are given about the validity of that pointer if it will be
	 * stored. Also, dereferencing the pointer and copying the object to a
	 * different location will result in undefined behavior at best, but will
	 * probably crash the program.
	 */
	Object *operator->() const;

private:
	ObjectPtr& operator=(Object *object);

	std::shared_ptr<Object *> _ptr;

};

}

#endif
