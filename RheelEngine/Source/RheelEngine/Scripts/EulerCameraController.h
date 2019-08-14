#ifndef EULERCAMERACONTROLLER_H_
#define EULERCAMERACONTROLLER_H_
#include "../_common.h"

#include "InputScript.h"
#include "../Camera.h"

namespace rheel {

class RE_API EulerCameraController : public InputScript {

public:
	void SetCamera(const std::string& camera);

	void PreOnUpdate() override;

private:
	void _Rotate(CameraPtr camera);

	std::string _camera;
};

}

#endif
