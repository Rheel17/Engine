#ifndef RIGIDBODYCOMPONENT_H_
#define RIGIDBODYCOMPONENT_H_
#include "../_common.h"

#include "../Component.h"
#include "../Physics/RigidShape.h"

namespace rheel {

/**
 * A component for Rigid Bodies. This component is required for the
 * built-in physics engine.
 */
class RE_API RigidbodyComponent : public Component {

public:
	void OnAdd() override;

	void OnUpdate() override;

	void OnRemove() override;

private:
	std::shared_ptr<btRigidBody> _body;

};

}

#endif
