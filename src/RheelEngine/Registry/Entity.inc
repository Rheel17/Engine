/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_ENTITY_INC
#define ENGINE_ENTITY_INC

#ifndef ENGINE_REGISTRY_H
#error "Do not include directly, include via Registry.h"
#endif

class RE_API Entity {
	friend class ComponentStorage;
	friend class Registry;

public:
	Entity(Entity* parent, Registry* registry, EntityId id, const Transform& transform) :
			transform(transform),
			_id(id),
			_parent(parent),
			_registry(registry) {}

	~Entity() = default;

	RE_NO_COPY(Entity);
	RE_NO_MOVE(Entity);

	Entity& AddChild(EntityId id, const Transform& transform = Transform());
	Entity& AddChild(const Transform& transform = Transform());
	void RemoveChild(Entity* child);

	std::span<Entity*> GetChildren();

	template<typename C, typename... Args>
	C& AddComponent(Args&&... args);

	template<ComponentBaseClass C>
	C* GetComponent();

	template<ComponentBaseClass C>
	const C* GetComponent() const;

	// returns true if a component was removed
	template<typename C>
	bool RemoveComponent();

	Transform transform;
	Transform AbsoluteTransform() const;

	Entity* GetParent();
	const Entity* GetParent() const;

	Scene& GetScene();
	const Scene& GetScene() const;

private:
	EntityId _id;
	Entity* _parent;
	Registry* _registry;

	// In release mode, we can probably win some performance by allocating small
	// entities completely on the stack. Small entities: <= 4 components.
// #if NDEBUG
// 	llvm::SmallVector<Component*, 4> _components{};
// #else
	std::vector<Component*> _components{};
	std::vector<Entity*> _children{};
// #endif

};

#endif
