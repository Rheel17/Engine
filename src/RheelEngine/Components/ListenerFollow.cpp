///*
// * Copyright (c) Levi van Rheenen. All rights reserved.
// */
//#include "ListenerFollow.h"
//
//#include "../Engine.h"
//
//namespace rheel {
//
//void ListenerFollow::SetCamera(std::string camera) {
//	_camera = std::move(camera);
//}
//
//void ListenerFollow::PostOnUpdate() {
//	// get the camera
//	Camera *camera = Parent().GetCamera(_camera);
//
//	if (!camera) {
//		return;
//	}
//
//	vec3 pos = camera->Position();
//	vec3 vel;
//
//	if (_has_prev_position) {
//		vel = pos - _prev_position;
//	} else {
//		vel = vec3(0, 0, 0);
//	}
//
//	Engine::GetAudioManager().GlobalListener().SetPosition(pos);
//	Engine::GetAudioManager().GlobalListener().SetVelocity(vel);
//	Engine::GetAudioManager().GlobalListener().SetOrientation(camera->Rotation());
//
//	_has_prev_position = true;
//	_prev_position = pos;
//}
//
//}
