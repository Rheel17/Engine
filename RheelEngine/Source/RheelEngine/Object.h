#ifndef OBJECT_H_
#define OBJECT_H_
#include "_common.h"

#include "Blueprint.h"

#include <memory>

namespace rheel {

class Scene;

class RE_API Object {
	friend class Scene;

public:
	enum EventType {
		ON_ADD, ON_REMOVE, ON_UPDATE, ON_UPDATE_RENDERER, ON_RENDER
	};

	/**
	 * Constructs a new object from the given blueprint.
	 */
	Object(const Blueprint& blueprint);

	/**
	 * Returns the parent object of this object, or nullptr if this object has
	 * no parent object.
	 */
	Object *ParentObject();

	/**
	 * Returns the parent scene of this object.
	 */
	Scene *ParentScene();

	/**
	 * Sets the position of this object.
	 */
	void SetPosition(const vec3& position);

	/**
	 * Sets the position of this object.
	 */
	void SetPosition(float x, float y, float z);

	/**
	 * Moves this object relative to its current position.
	 */
	void Move(const vec3& position);

	/**
	 * Moves this object relative to its current position.
	 */
	void Move(float x, float y, float z);

	/**
	 * Returns the position of this object.
	 */
	const vec3& Position() const;

	/**
	 * Sets the rotation of this object.
	 */
	void SetRotation(const quat& rotation);

	/**
	 * Rotates this object relative to its current rotation.
	 */
	void Rotate(const quat& rotation);

	/**
	 * Returns the rotation of this object.
	 */
	const quat& Rotation() const;

	/**
	 * Sets the scale of this object.
	 */
	void SetScale(const vec3& scale);

	/**
	 * Sets the scale of this object.
	 */
	void SetScale(float x, float y, float z);

	/**
	 * Sets the scale of this object.
	 */
	void SetScale(float scale);

	/**
	 * Returns the scale of this object.
	 */
	const vec3& Scale() const;

	/**
	 * Fires an event of the given type to its components. A second parameter
	 * can be set to false if this event should not be fired to children of this
	 * object. By default, this will fire the same event in its children.
	 * NOTE: If recursive is true, the event will FIRST be fired in its
	 *       children, THEN in its own components.
	 */
	void FireEvent(EventType type, bool recursive = true);

	/**
	 * Returns a shader pointer to the component of the given type. A nullptr
	 * shared pointer is returned if no such component exists in this object.
	 */
	template<typename T>
	std::shared_ptr<T> GetComponent() {
		static_assert(std::is_base_of<Component, T>::value, "Type must be a component");

		for (auto component : _components) {
			if (auto ptr = std::dynamic_pointer_cast<T>(component)) {
				return ptr;
			}
		}

		return nullptr;
	}

private:
	void _SetParentScene(Scene *scene);

	Scene *_parent_scene = nullptr;
	Object *_parent_object = nullptr;

	bool _alive = true;

	vec3 _position = vec3(0, 0, 0);
	quat _rotation = quat(1, 0, 0, 0);
	vec3 _scale = vec3(1, 1, 1);

	std::vector<ComponentPtr> _components;
	std::vector<Object *> _children;
};

}

#endif
