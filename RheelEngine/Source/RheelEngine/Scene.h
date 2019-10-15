/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef SCENE_H_
#define SCENE_H_
#include "_common.h"

#include "RigidTransform.h"

namespace rheel {

class Entity;

class RE_API Scene {
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
	 * Updates the scene and all its entities.
	 */
	void Update();

private:
	Entity *_root_entity;

};

}

#endif
