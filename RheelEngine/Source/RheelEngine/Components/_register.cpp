#include "../Engine.h"

#include "ModelRenderComponent.h"
#include "RigidbodyComponent.h"

namespace rheel {

void registerComponents() {
	Engine::RegisterComponent<ModelRenderComponent>(Component::NAME_MODELRENDER);
	Engine::RegisterComponent<RigidbodyComponent>(Component::NAME_RIGIDBODY);
}

}
