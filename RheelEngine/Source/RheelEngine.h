/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_RHEELENGINE_H_
#define RHEELENGINE_RHEELENGINE_H_

// includes for all required header files
// TODO: do this automatically?

#include "RheelEngine/Engine.h"
#include "RheelEngine/ResourceManager.h"

#include "RheelEngine/Components/CollisionComponent.h"
#include "RheelEngine/Components/ModelRenderComponent.h"
#include "RheelEngine/Components/RigidBodyComponent.h"

#include "RheelEngine/Scripts/EulerCameraController.h"
#include "RheelEngine/Scripts/PhysicsScene.h"

#include "RheelEngine/UI/Elements/ColorElement.h"
#include "RheelEngine/UI/Elements/EmptyElement.h"
#include "RheelEngine/UI/Elements/SceneElement.h"
#include "RheelEngine/UI/Elements/TextElement.h"

// includes for entry point
#include <type_traits>

// entry point
namespace rheel {

template<typename GameClass>
void __Entry__() {
	static_assert(std::is_default_constructible<GameClass>::value, "Game Class must be default-constructible");
	static_assert(std::is_base_of<rheel::Game, GameClass>::value, "Game Class must derive from re::Base");
	Engine::_Initialize();
	Game* game = new GameClass;
	Engine::_Run(game);
	delete game;
	Engine::_Terminate();
}

}

// macro for entry point
#define RHEEL_ENGINE_ENTRY(GameClass)       \
	int main(int argc, char* argv[]) {      \
		__Entry__<GameClass>();				\
	}

#endif
