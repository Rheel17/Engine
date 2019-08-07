#include "ObjectPtr.h"

#include "Object.h"

namespace rheel {

ObjectPtr::ObjectPtr() :
		_ptr(std::make_shared<Object *>()) {}

Object *ObjectPtr::operator->() const {
	return *_ptr;
}

ObjectPtr& ObjectPtr::operator=(Object *object) {
	*_ptr = object;
	return *this;
}

}
