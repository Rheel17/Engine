// /*
//  * Copyright (c) Levi van Rheenen. All rights reserved.
//  */
// #ifndef RHEELENGINE_COLLISIONCOMPONENT_H
// #define RHEELENGINE_COLLISIONCOMPONENT_H
// #include "../_common.h"
//
// #include "../Component.h"
//
// namespace rheel {
//
// /**
//  * Handles collision events between two objects. In order for this component to
//  * work, a RigidBodyComponent must be added before this component. Only
//  * collisions with other objects having a CollisionComponent will be detected.
//  */
// class RE_API CollisionComponent : public Component {
//
// public:
// 	// gen_component_id
// 	static constexpr const ComponentId id = 65281;
// 	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;
//
// 	CollisionComponent() = default;
// 	~CollisionComponent() override = default;
//
// 	RE_NO_COPY(CollisionComponent);
// 	RE_DEFAULT_MOVE(CollisionComponent);
//
// 	virtual void OnCollisionStart(CollisionComponent& other) {}
// 	virtual void OnCollisionEnd(CollisionComponent& other) {}
//
// };
//
// }
//
// #endif
