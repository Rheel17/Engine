/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef OBJECT_H_
#define OBJECT_H_
#include "_common.h"

#include "SceneDescription.h"

namespace rheel {

class SceneOld;

class RE_API Object {
	friend class SceneOld;
	friend class ObjectPtr;

	RE_NO_COPY(Object);

public:
	enum EventType {
		ON_ADD, ON_REMOVE, ON_UPDATE, ON_UPDATE_RENDERER
	};

	/**
	 * Constructs a new object from the given blueprint.
	 */
	Object(const Blueprint& blueprint);

	/**
	 * Moves this object.
	 */
	Object(Object&& object) noexcept;

	/**
	 * Moves this object.
	 */
	Object& operator=(Object&& object);

	/**
	 * Returns the parent object of this object, or nullptr if this object has
	 * no parent object.
	 */
	Object& ParentObject();

	/**
	 * Returns the parent scene of this object.
	 */
	SceneOld *ParentScene();

	/**
	 * Returns the name of the blueprint that constructed this object.
	 */
	const std::string& BlueprintName() const;

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
	 * Fires an event of the given type to its components. A second parameter
	 * can be set to false if this event should not be fired to children of this
	 * object. By default, this will fire the same event in its children.
	 * NOTE: If recursive is true, the event will first be fired in its
	 *       children, then in its own components.
	 */
	void FireEvent(EventType type, bool recursive = true);

	/**
	 * Adds a component to this object. A reference to the component is
	 * returned. After the object is initialized, the "OnAdd" must be called
	 * manually.
	 */
	template<typename T>
	T& AddComponent() {
		static_assert(std::is_base_of<ComponentOld, T>::value, "Class is not derived from Component");
		static_assert(std::is_default_constructible<T>::value, "Components must be Default-Constructable");

		std::unique_ptr<T> ptr = std::make_unique<T>();
		T& ref = *ptr;

		ptr->_parent_object = this;
		_components.push_back(std::move(ptr));

		return ref;
	}

	/**
	 * Removes a component from this object.
	 */
	void RemoveComponent(ComponentOld& component);

	/**
	 * Returns a pointer to the component of the given type. A nullptr is
	 * returned if no such component exists in this object. If multiple
	 * components of the same type exist, the first one added is returned.
	 */
	template<typename T>
	T *GetComponent() {
		static_assert(std::is_base_of<ComponentOld, T>::value, "Type must be a component");

		for (const auto& component : _components) {
			if (auto ptr = dynamic_cast<T *>(component.get())) {
				return ptr;
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T *> GetAllComponentsOfType() {
		static_assert(std::is_base_of<ComponentOld, T>::value, "Type must be a component");
		std::vector<T *> vec;

		for (const auto& component : _components) {
			if (auto ptr = dynamic_cast<T *>(component.get())) {
				vec.push_back(ptr);
			}
		}

		return vec;
	}

private:
	void _SetParentScene(SceneOld *scene);

	std::string _blueprint_name;

	SceneOld *_parent_scene = nullptr;
	Object *_parent_object = nullptr;

	bool _alive = true;
	float _time = 0.0f;
	float _dt = 1.0f / 60.0f;

	vec3 _position = vec3(0, 0, 0);
	quat _rotation = quat(1, 0, 0, 0);

	std::vector<std::unique_ptr<ComponentOld>> _components;
	std::vector<std::unique_ptr<Object>> _children;
};

}

#endif
