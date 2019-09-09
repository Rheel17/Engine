#ifndef RIGIDBODYCOMPONENT_H_
#define RIGIDBODYCOMPONENT_H_
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include "../Component.h"

namespace rheel {

/**
 * A component for Rigid Bodies. This component is required for the
 * built-in physics engine.
 */
class RE_API RigidbodyComponent : public Component {
	COMPONENT_INIT(RigidbodyComponent)

private:
	RigidbodyComponent(const RigidbodyComponent& component);

public:
	RigidbodyComponent() = default;

	void OnAdd() override;

	void OnUpdate() override;

	void OnRemove() override;

};

}

#endif
