/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef ENTITY_H_
#define ENTITY_H_
#include "_common.h"

#include "RigidTransform.h"
#include "Scene.h"

namespace rheel {

class ComponentBase;

class RE_API Entity {
	RE_NO_MOVE(Entity);
	RE_NO_COPY(Entity);

private:
	struct _ComponentBaseDeleter {
		void operator()(ComponentBase *ptr) const;
	};

	using _ComponentBasePtr = std::unique_ptr<ComponentBase, _ComponentBaseDeleter>;

public:
	/**
	 * Adds an empty child entity.
	 */
	Entity *AddChild(RigidTransform transform = RigidTransform());

	/**
	 * Adds a component to this entity. The entity will take ownership of the
	 * pointer. If the component was already added to any entity (including this
	 * one), this will cause undefined behavior.
	 *
	 * If this entity is already active, the component will be activated
	 * immediately.
	 */
	void AddComponent(ComponentBase* component);

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
	 * first one added is returned. If no components can be found, nullptr is
	 * returned.
	 */
	template<typename T>
	T *GetComponent() {
		static_assert(std::is_base_of_v<ComponentBase, T>, "Type must be a component");

		for (const auto& component : _components) {
			if (auto ptr = dynamic_cast<T *>(component.get())) {
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

		for (const auto& component : _components) {
			if (auto ptr = dynamic_cast<T *>(component.get())) {
				vec.push_back(ptr);
			}
		}

		return vec;
	}

	// The root transform of this entity.
	RigidTransform transform;

	// The parent scene of this entity.
	Scene * const scene;

	// The parent entity, or nullptr if the entity is a root entity.
	Entity * const parent;

private:
	Entity(Scene *scene, RigidTransform transform = RigidTransform());
	Entity(Entity *parent, RigidTransform transform = RigidTransform());

	bool _alive;
	float _time;
	float _dt;

	std::vector<_ComponentBasePtr> _components;

};

}

#endif
