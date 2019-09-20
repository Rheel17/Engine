#ifndef RIGIDBODYCOMPONENT_H_
#define RIGIDBODYCOMPONENT_H_
#include "../_common.h"

#include "../Component.h"
#include "../PhysicsShape.h"

namespace rheel {

/**
 * A component for Rigid Bodies. This component is required for the
 * built-in physics engine.
 */
class RE_API RigidBodyComponent : public Component {
	COMPONENT_INIT(RigidBodyComponent)

private:
	RigidBodyComponent(const RigidBodyComponent& component);

public:
	RigidBodyComponent() = default;

	void OnAdd() override;

	void OnUpdate() override;

	void OnRemove() override;

	void SetShape(PhysicsShape shape);

	void SetMass(float mass);

	void ApplyForce(const vec3& force);

	void ApplyImpulse(const vec3& impulse);

private:
	std::unique_ptr<btMotionState> _motion_state;
	std::unique_ptr<btRigidBody> _body;

	PhysicsShape _shape;
	float _mass = 0.0f;

};

}

#endif
