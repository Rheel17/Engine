#ifndef RIGIDBODYCOMPONENT_H_
#define RIGIDBODYCOMPONENT_H_
#include "../_common.h"

#include "../Component.h"
#include "../Physics/RigidBody.h"

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

private:
	std::unique_ptr<RigidBody> _body;

};

}

#endif
