#include "../Components/ModelRenderComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Engine.h"


namespace rheel {

void registerComponents() {
	Engine::RegisterComponent<ModelRenderComponent>(Component::NAME_MODELRENDER);
	Engine::RegisterComponent<RigidbodyComponent>(Component::NAME_RIGIDBODY);
}

}
