#include "RigidbodyComponent.h"

#include <btBulletDynamicsCommon.h>

#include "../Object.h"

namespace rheel {

template<typename Deleter>
static btRigidBody *get(const std::unique_ptr<void, Deleter>& ptr) {
	return static_cast<btRigidBody *>(ptr.get());
}

void RigidbodyComponent::_DeleterBody::operator()(void *ptr) {
	delete static_cast<btRigidBody *>(ptr);
}

RigidbodyComponent::RigidbodyComponent(const RigidbodyComponent& component) {}

void RigidbodyComponent::OnAdd() {
	// TODO implement
}

void RigidbodyComponent::OnUpdate() {
	if (!_body) {
		return;
	}

	auto origin = get(_body)->getWorldTransform().getOrigin();
	auto rotation = get(_body)->getWorldTransform().getRotation();

	Parent()->SetPosition(origin.x(), origin.y(), origin.z());
	Parent()->SetRotation(quat(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
}

void RigidbodyComponent::OnRemove() {
	if (!_body) {
		return;
	}

	// TODO implement
}

}
