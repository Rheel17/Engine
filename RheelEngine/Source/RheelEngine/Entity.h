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

private:
	struct _ComponentBaseDeleter {
		void operator()(ComponentBase *ptr) const;
	};

	using _ComponentBasePtr = std::unique_ptr<ComponentBase, _ComponentBaseDeleter>;

public:
	/**
	 * Adds an empty child entity.
	 */
	Entity *AddChild(std::string name, RigidTransform transform = RigidTransform());

	/**
	 * Removes a child from this entity.
	 */
	void RemoveChild(Entity *entity);

	/**
	 * Returns the child entity with the name, if it exists. Returns nullptr
	 * otherwise.
	 */
	Entity *GetChild(const std::string& name);

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

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		T *c = component.get();

		_components.push_back(std::move(component));
		c->_entity = this;
		c->Activate();

		return &c;
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

	std::vector<std::unique_ptr<Entity>> _children;
	std::vector<_ComponentBasePtr> _components;

};

}

#endif
