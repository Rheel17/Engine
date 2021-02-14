/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_SCENE_H
#define RHEELENGINE_SCENE_H
#include "_common.h"

#include "Game.h"
#include "Assets/Image.h"
#include "Registry/Registry.h"

namespace rheel {

class RE_API Scene {
	friend class Game;

public:
	~Scene() = default;

	RE_NO_MOVE(Scene);
	RE_NO_COPY(Scene);

	/**
	 * Returns the game of this scene.
	 */
	Game& GetGame();

	Registry& GetRegistry();
	const Registry& GetRegistry() const;

	/**
	 * Adds an empty entity to the scene. This function returns a handle to the
	 * created entity.
	 */
	Entity& AddEntity(EntityId id, const Transform& transform = Transform());

	/**
	 * Adds an empty entity to the scene. This function returns a handle to the
	 * created entity.
	 */
	Entity& AddEntity(const Transform& transform = Transform());

	/**
	 * Removes an entity from the scene. If it is still active, it will be
	 * killed first. If the entity is a child of another entity, the entity will
	 * be removed from its parent.
	 */
	void RemoveEntity(EntityId entity);

	template<typename C>
	C* GetRootComponent() {
		return _registry.GetRootEntity()->GetComponent<C>();
	}

	template<typename C>
	const C* GetRootComponent() const {
		return _registry.GetRootEntity()->GetComponent<C>();
	}

	/**
	 * Adds a component to the scene root, instead of a specific object in the
	 * scene. Use this only for scene-wide components.
	 */
	template<typename C, typename... Args>
	C& AddRootComponent(Args&&... args) {
		return _registry.GetRootEntity()->AddComponent<C>(std::forward<Args>(args)...);
	}

	/**
	 * Removes a component from the scene root.
	 */
	template<typename C>
	void RemoveRootComponent() {
		_registry.GetRootEntity()->RemoveComponent<C>();
	}

	/**
	 * Updates the scene and all its entities.
	 */
	void Update(float time, float dt);

private:
	explicit Scene(Game& game);

	Game& _game;
	Registry _registry;
};

}

#endif
