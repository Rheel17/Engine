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
	COMPONENT_INIT(RigidbodyComponent)

private:
	struct _DeleterBody { void operator()(void *ptr); };

private:
	RigidbodyComponent(const RigidbodyComponent& component);

public:
	RigidbodyComponent() = default;

	void OnAdd() override;

	void OnUpdate() override;

	void OnRemove() override;

private:
	std::unique_ptr<void, _DeleterBody> _body;

};

}

#endif
