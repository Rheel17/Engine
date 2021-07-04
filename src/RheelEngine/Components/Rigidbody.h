/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_RIGIDBODY_H
#define RHEELENGINE_RIGIDBODY_H
#include "../_common.h"

#include "../Component.h"
#include "../PhysicsShape.h"

namespace rheel {

/**
 * A component for Rigid Bodies. This component is required for the
 * built-in physics engine.
 */
class RE_API RigidBody : public Component {

public:
	// gen_component_id
	static constexpr const ComponentId id = 65290;
	static constexpr const ComponentFlags flags = ComponentFlags::BUILTIN;

	explicit RigidBody(PhysicsShape shape, float mass = 0.0f, float bounciness = 0.5f);
	~RigidBody() = default;

	RE_NO_COPY(RigidBody);

	RigidBody(RigidBody&& rb) noexcept;
	RigidBody& operator=(RigidBody&& rb) = delete;

protected:
	void OnActivate() override;
	void OnDeactivate() override;

public:
	void Update() override;

	void SetShape(PhysicsShape shape);

	void SetMass(float mass);

	void SetBounciness(float bounciness);

	void ApplyForce(const vec3& force);

	void ApplyImpulse(const vec3& impulse);

private:
	struct data {
		std::unique_ptr<btMotionState> motion_state;
		std::unique_ptr<btRigidBody> body;
		std::unique_ptr<btTransform> last_transform_update;

		PhysicsShape shape;
		float mass = 0.0f;
		float bounciness = 0.5f;
	} _data;

};

}

#endif
