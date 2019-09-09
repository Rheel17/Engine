#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_
#include "../_common.h"

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace rheel {

class RE_API RigidBody {

public:
	RigidBody();

private:
	std::unique_ptr<btRigidBody> _body;

};

}

#endif
