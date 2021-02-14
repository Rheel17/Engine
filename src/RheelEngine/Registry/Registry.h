/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_REGISTRY_H
#define ENGINE_REGISTRY_H
#include "../_common.h"

#include "ComponentStorage.h"
#include "EntityStorage.h"
#include "EntityId.h"
#include "../Transform.h"

#if NDEBUG
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#else
#include <map>
#endif

namespace rheel {

class Registry;
class Scene;

class Entity {
	friend class ComponentStorage;
	friend class Registry;

public:
	Entity(Entity* parent, Registry* registry, const Transform& transform) :
			transform(transform),
			_parent(parent),
			_registry(registry) {}

	~Entity() = default;

	RE_NO_COPY(Entity);
	RE_NO_MOVE(Entity);

	template<typename C, typename... Args>
	C& AddComponent(Args&&... args);

	template<ComponentBaseClass C>
	C* GetComponent();

	template<ComponentBaseClass C>
	const C* GetComponent() const;

	template<typename C>
	void RemoveComponent();

	Transform transform;
	Transform AbsoluteTransform() const;

	Entity* GetParent();
	const Entity* GetParent() const;

	Scene& GetScene();
	const Scene& GetScene() const;

private:
	Entity* _parent;
	Registry* _registry;

	// In release mode, we can probably win some performance by allocating small
	// entities completely on the stack. Small entities: <= 4 components.
#if NDEBUG
	llvm::SmallVector<Component*, 4> _components{};
#else
	std::vector<Component*> _components{};
#endif

};

class RE_API Registry {
	friend class Entity;

	static constexpr std::size_t _user_components_reserve_size = 256;

public:
	explicit Registry(Scene* scene);
	~Registry() = default;

	RE_NO_COPY(Registry);
	RE_DEFAULT_MOVE(Registry);

	Entity& AddEntity(EntityId id, const Transform& transform);
	Entity& AddChildEntity(Entity* parent, EntityId id, const Transform& transform);
	Entity& AddEntity(const Transform& transform);
	Entity& AddChildEntity(Entity* parent, const Transform& transform);

	void EraseEntity(EntityId id);

	Entity* GetEntity(EntityId id);
	const Entity* GetEntity(ConstEntityId id) const;

	Entity* GetRootEntity();
	const Entity* GetRootEntity() const;

	template<ComponentClass C, typename... Args>
	C& AddComponent(EntityId entity_id, Args&&... args) {
		return AddComponent<C>(GetEntity(entity_id), std::forward<Args>(args)...);
	}

	template<ComponentClass C, typename... Args>
	C& AddComponent(Entity* entity, Args&&... args) {
		// create the component
		C* component = _component_storage<C>().template NewInstance<C>(args...);
		component->_entity = entity;
		component->_index_in_entity = static_cast<std::uint16_t>(entity->_components.size());
		entity->_components.push_back(component);

		// activate the component
		static_cast<Component*>(component)->OnActivate();

		return *component;
	}

	template<ComponentClass C>
	void RemoveComponent(std::size_t instance) {
		auto[entity_index, index_in_entity] = _component_storage<C>().template RemoveInstance<C>(_entities, instance);

		auto& entity = _entities[entity_index];
		entity._components.erase(entity._components.begin() + index_in_entity);
	}

	template<ComponentClass C, typename F>
	auto ForAll(F f) {
		return _component_storage<C>().template ForAll<C>(f);
	}

	template<ComponentClass C, typename F>
	auto ForAll(F f) const {
		return _component_storage<C>().template ForAll<C>(f);
	}

	template<typename F>
	void ForAllComponents(F f) {
		for (auto& components : _builtin_components) {
			components.ForAllGeneric(f);
		}

		for (auto& components : _user_components) {
			components.ForAllGeneric(f);
		}
	}

private:
	// mapping Entity Ids to indices for this registry
#if NDEBUG
	llvm::DenseMap<std::uint64_t, std::size_t> _id_to_index_map;
#else
	std::map<std::uint64_t, std::size_t> _id_to_index_map;
#endif

	std::size_t _entity_index(std::uint64_t id) const {
#ifdef NDEBUG
		return _id_to_index_map.find(id)->getSecond();
#else
		auto iter = _id_to_index_map.find(id);

		if (iter == _id_to_index_map.end()) {
			throw std::runtime_error("Entity id does not exist in registry");
		}

		return iter->second;
#endif
	}

	// entities and their components
	EntityStorage<Entity> _entities;
	std::vector<ComponentStorage> _builtin_components;
	std::vector<ComponentStorage> _user_components;

	template<ComponentClass C>
	ComponentStorage& _component_storage() {
		if constexpr (ComponentWithFlag<C, ComponentFlags::BUILTIN>) {
			return _builtin_components[C::id];
		} else {
			if constexpr (C::id >= _user_components_reserve_size) {
				if (_user_components.size() <= C::id) {
					_user_components.resize(C::id + 1);
				}
			}

			return _user_components[C::id];
		}
	}

	// the parent scene
	Scene* _scene;
	Entity* _root;
};

template<ComponentBaseClass C>
C* Entity::GetComponent() {
	for (auto* component : _components) {
		if (auto* c = dynamic_cast<C*>(component)) {
			return c;
		}
	}

	return nullptr;
}

template<ComponentBaseClass C>
const C* Entity::GetComponent() const {
	for (auto* component : _components) {
		if (const auto* c = dynamic_cast<const C*>(component)) {
			return c;
		}
	}

	return nullptr;
}

template<typename C, typename... Args>
C& Entity::AddComponent(Args&&... args) {
	return _registry->AddComponent<C>(this, std::forward<Args>(args)...);
}

template<typename C>
void Entity::RemoveComponent() {
	auto* instance = GetComponent<C>();
	_registry->RemoveComponent<C>(instance->_index);
}

}

#endif
