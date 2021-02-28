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
#include "../Components/InputComponent.h"

// #if NDEBUG
// #include <llvm/ADT/DenseMap.h>
// #include <llvm/ADT/SmallVector.h>
// #else
#include <map>
// #endif

namespace rheel {

class Registry;
class Scene;

template<typename, typename...>
class MultiComponentView;

#include "Entity.inc"

class RE_API Registry {
	friend class Entity;

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
		constexpr ComponentId id = C::id;

		// create the component
		C* component = _components[id].template NewInstance<C>(args...);
		component->_entity = entity;
		component->_index_in_entity = static_cast<std::uint16_t>(entity->_components.size());
		entity->_components.push_back(component);

		// activate the component
		static_cast<Component*>(component)->OnActivate();

		// If the component is an input component, add it to the input
		// components
		if constexpr (std::is_base_of_v<InputComponent, C>) {
			_input_components.push_back(component);
		}

		// update the ranges
		if constexpr (ComponentWithFlag<C, ComponentFlags::BUILTIN>) {
			_builtin_start = std::min(_builtin_start, id);
			_builtin_end = std::max(_builtin_end, static_cast<ComponentId>(id + 1));
		} else {
			_user_defined_start = std::min(_user_defined_start, id);
			_user_defined_end = std::max(_user_defined_end, static_cast<ComponentId>(id + 1));
		}

		return *component;
	}

	template<ComponentClass C>
	void RemoveComponent(std::size_t instance) {
		// If the component is an input component, remove it from the input
		// components
		if constexpr (std::is_base_of_v<InputComponent, C>) {
			std::erase(_input_components, _components[C::id][instance]);
		}

		auto[entity_index, index_in_entity] = _components[C::id].template RemoveInstance<C>(_entities, instance);

		auto& entity = _entities[entity_index];
		entity._components.erase(entity._components.begin() + index_in_entity);
	}

	void UpdateComponents(float time, float dt);

	template<ComponentClass C>
	auto GetComponents() {
		return ComponentView<C>(_components[C::id]);
	}

	template<ComponentClass C>
	auto GetComponents() const {
		return ComponentView<C>(_components[C::id]);
	}

	template<ComponentClass C1, ComponentClass C2, ComponentClass... Cn>
	auto GetComponents() {
		return MultiComponentView<Registry, C1, C2, Cn...>(this);
	}

	template<ComponentClass C1, ComponentClass C2, ComponentClass... Cn>
	auto GetComponents() const {
		return MultiComponentView<const Registry, C1, C2, Cn...>(this);
	}

	std::span<InputComponent*> GetInputComponents();

private:
	// mapping Entity Ids to indices for this registry
// #if NDEBUG
// 	llvm::DenseMap<std::uint64_t, std::size_t> _id_to_index_map;
// #else
	std::map<std::uint64_t, std::size_t> _id_to_index_map;
// #endif

	std::size_t _entity_index(std::uint64_t id) const {
// #ifdef NDEBUG
// 		return _id_to_index_map.find(id)->getSecond();
// #else
		auto iter = _id_to_index_map.find(id);

		if (iter == _id_to_index_map.end()) {
			throw std::runtime_error("Entity id does not exist in registry");
		}

		return iter->second;
// #endif
	}

	// entities and their components
	EntityStorage<Entity> _entities;
	std::vector<ComponentStorage> _components{ 65536 };

	// input components
	std::vector<InputComponent*> _input_components{};

	// the parent scene
	Scene* _scene;
	Entity* _root;

	// update range
	ComponentId _user_defined_start = 0;
	ComponentId _user_defined_end = _user_defined_start;
	ComponentId _builtin_start = 65536 - 256;
	ComponentId _builtin_end = _builtin_start;

};

}

#include "EntityImpl.inc"
#include "MultiComponentView.inc"

#endif
