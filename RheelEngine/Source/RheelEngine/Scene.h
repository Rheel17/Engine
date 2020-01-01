/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SCENE_H_
#define SCENE_H_
#include "_common.h"

#include "Entity.h"

namespace rheel {

class Camera;
class Light;
class ComponentInputProxy;

class RE_API Scene {
	friend class Camera;
	friend class Light;
	friend class ComponentInputProxy;

	RE_NO_MOVE(Scene);
	RE_NO_COPY(Scene);

public:
	Scene();
	~Scene();

	/**
	 * Adds an empty entity to the scene. This function returns a pointer to the
	 * created entity.
	 */
	Entity *AddEntity(std::string name, RigidTransform transform = RigidTransform());

	/**
	 * Creates a unique entity name with the given prefix
	 */
	std::string UniqueEntityName(const std::string& prefix);

	/**
	 * Removes an entity from the scene. If it is still active, it will be
	 * killed first. If the entity is a child of another entity, the entity will
	 * be removed from its parent.
	 */
	void RemoveEntity(Entity *entity);

	/**
	 * Finds and returns the first found entity with the given name. The
	 * 'recursive' parameter (default: true) can be specified to turn on or off
	 * recursive search. With recursive search, the root entities are first
	 * searched in order of their creation. If no entity of that name is found,
	 * the children of the entities are searched.
	 *
	 * If no entity with the name given can be found, nullptr is returned.
	 */
	Entity *FindEntity(const std::string& name, bool recursive = true);

	/**
	 * Returns a vector of all root-level entities in the scene. No child
	 * entities are returned.
	 */
	const std::vector<Entity *>& GetEntities() const;

	/**
	 * Adds a component to the scene root, instead of a specific object in the
	 * scene. Use this only for scene-wide components.
	 */
	template<typename T, typename... Args>
	T *AddRootComponent(Args&&... args) {
		return _root_entity->AddComponent<T>(args...);
	}

	/**
	 * Removes a component from the scene root.
	 */
	void RemoveRootComponent(ComponentBase *component);

	/**
	 * Returns the root component of the given type, if this scene has one
	 * attached. If multiple components of the same type are present in the
	 * scene, the first one added is returned. If no component is found, nullptr
	 * is returned.
	 */
	template<typename T>
	T *GetRootComponent() {
		return _root_entity->GetComponent<T>();
	}

	/**
	 * Returns all root components of the given type.
	 */
	template<typename T>
	std::vector<T *> GetAllRootComponentsOfType() {
		return _root_entity->GetAllComponentsOfType<T>();
	}

	/**
	 * Returns the camera of the given name. If no camera exists with that name,
	 * nullptr is returned.
	 */
	Camera *GetCamera(const std::string& name);

	/**
	 * Returns the light of the given name. If no light exists with that name,
	 * nullptr is returned.
	 */
	Light *GetLight(const std::string& name);

	/**
	 * Returns a vector of all lights in the scene.
	 */
	const std::vector<Light *>& GetLights();

	/**
	 * Returns the components registered to receive input events
	 */
	const std::vector<ComponentInputProxy *>& GetInputComponents() const;

	/**
	 * Updates the scene and all its entities.
	 */
	void Update(float time, float dt);

private:
	Entity *_root_entity;

	std::unordered_map<std::string, Camera *> _cameras;
	std::vector<Light *> _lights;
	std::vector<Entity *> _entities;
	std::vector<ComponentInputProxy *> _input_components;

};

}

#endif
