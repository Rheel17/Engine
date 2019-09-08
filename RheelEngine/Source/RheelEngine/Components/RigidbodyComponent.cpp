#include "RigidbodyComponent.h"

#include "../Object.h"

namespace rheel {

void RigidbodyComponent::OnAdd() {
//	if (!_shape) {
//		return;
//	}
}

void RigidbodyComponent::OnUpdate() {
	if (!_body) {
		return;
	}

	auto origin = _body->getWorldTransform().getOrigin();
	auto rotation = _body->getWorldTransform().getRotation();

	Parent()->SetPosition(origin.x(), origin.y(), origin.z());
	Parent()->SetRotation(quat(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
}

void RigidbodyComponent::OnRemove() {
	if (!_body) {
		return;
	}
}

}
