/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef ENTITY_H_
#define ENTITY_H_
#include "_common.h"

#include "RigidTransform.h"

namespace rheel {

class ComponentBase;
class Scene;

class RE_API Entity {
	RE_NO_MOVE(Entity);
	RE_NO_COPY(Entity);

	friend class Scene;

public:
	~Entity();

	/**
	 * Adds an empty child entity. If this entity already has a direct child
	 * with the given name, nothing is done, and nullptr is returned.
	 */
	Entity *AddChild(std::string name, RigidTransform transform = RigidTransform());

	/**
	 * Creates a unique child name starting with the given prefix.
	 */
	std::string UniqueChildName(const std::string& prefix);

	/**
	 * Removes a child from this entity. If the entity is not a direct child of
	 * this entity, but is a descendant, it is removed from its direct parent.
	 * If the entity is not a descendant of this entity, nothing is done.
	 */
	void RemoveChild(Entity *entity);

	/**
	 * Finds and returns the first found child with the given name. The
	 * 'recursive' parameter (default: true) can be specified to turn on or off
	 * recursive search. With recursive search, the direct children are first
	 * searched in order of their creation. If no entity of that name is found,
	 * the children of the children are searched.
	 *
	 * If no entity with the name given can be found, nullptr is returned.
	 */
	Entity *FindChild(const std::string& name, bool recursive = true);

	/**
	 * Returns true of the base entity is in the parent chain of this entity.
	 */
	bool IsDescendantOf(Entity *base);

	/**
	 * Adds a component to this entity. The entity will take ownership of the
	 * pointer. If the component was already added to any entity (including this
	 * one), this will cause undefined behavior.
	 *
	 * If this entity is already active, the component will be activated
	 * immediately.
	 */
	template<typename T, typename... Args>
	T *AddComponent(Args&&... args) {
		static_assert(std::is_base_of_v<ComponentBase, T>, "Type must extend ComponentBase");

		T *c = new T(std::forward<Args>(args)...);

		_components.push_back(c);
		c->_entity = this;
		c->Activate();

		return c;
	}

	/**
	 * Removes a component from this entity. This has no effect if the component
	 * is not part of this entity.
	 *
	 * Calling this method deactivates the component.
	 */
	void RemoveComponent(ComponentBase *component);

	/**
	 * Returns the component of the given type, if this entity has one attached.
	 * If multiple components of the same type are present in this entity, the
	 * first one added is returned. If no component can be found, nullptr is
	 * returned.
	 */
	template<typename T>
	T *GetComponent() {
		static_assert(std::is_base_of_v<ComponentBase, T>, "Type must be a component");

		for (auto component : _components) {
			if (auto ptr = dynamic_cast<T *>(component)) {
				return ptr;
			}
		}

		return nullptr;
	}

	/**
	 * Returns all components of the given type.
	 */
	template<typename T>
	std::vector<T *> GetAllComponentsOfType() {
		static_assert(std::is_base_of_v<ComponentBase, T>, "Type must be a component");
		std::vector<T *> vec;

		for (auto component : _components) {
			if (auto ptr = dynamic_cast<T *>(component)) {
				vec.push_back(ptr);
			}
		}

		return vec;
	}

	/**
	 * Calculates the resulting transform after applying this Entity's parent's
	 * transforms and applying its own transform.
	 */
	RigidTransform CalculateResultingTransform() const;

	/**
	 * Updates this entity and all its children and components.
	 */
	void Update();

	// the name of this entity
	const std::string name;

	// The root transform of this entity.
	RigidTransform transform;

	// The parent scene of this entity.
	Scene * const scene;

	// The parent entity, or nullptr if the entity is a root entity.
	Entity * const parent;

private:
	Entity(std::string name, Scene *scene, RigidTransform transform = RigidTransform());
	Entity(std::string name, Entity *parent, RigidTransform transform = RigidTransform());

	bool _alive = true;
	float _time = 0.0f;
	float _dt = 0.0f;

	std::unordered_set<std::string> _child_names;
	std::vector<std::unique_ptr<Entity>> _children;
	std::vector<ComponentBase *> _components;

};

}

#endif
