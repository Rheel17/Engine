/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_RHEELENGINE_H
#define RHEELENGINE_RHEELENGINE_H

// includes for all required header files
// BEGIN GENERATED INCLUDES
#include "RheelEngine/Engine.h"
#include "RheelEngine/Animator/ConstantInterpolator.h"
#include "RheelEngine/Animator/CosineInterpolator.h"
#include "RheelEngine/Animator/CubicInterpolator.h"
#include "RheelEngine/Animator/LinearInterpolator.h"
#include "RheelEngine/Animator/SplineInterpolator.h"
#include "RheelEngine/Animator/TransitionInterpolator.h"
#include "RheelEngine/Assets/Generators/StaticModelGeneratorBox.h"
#include "RheelEngine/Assets/Generators/StaticModelGeneratorCapsule.h"
#include "RheelEngine/Assets/Generators/StaticModelGeneratorSphere.h"
#include "RheelEngine/Components/AnimatorComponent.h"
#include "RheelEngine/Components/CollisionComponent.h"
#include "RheelEngine/Components/DirectionalLight.h"
#include "RheelEngine/Components/EulerController.h"
#include "RheelEngine/Components/ListenerFollow.h"
#include "RheelEngine/Components/ModelRenderComponent.h"
#include "RheelEngine/Components/PerspectiveCamera.h"
#include "RheelEngine/Components/PhysicsScene.h"
#include "RheelEngine/Components/PointLight.h"
#include "RheelEngine/Components/Rigidbody.h"
#include "RheelEngine/Components/Skybox.h"
#include "RheelEngine/Components/SpotLight.h"
#include "RheelEngine/Components/VoxelRenderComponent.h"
#include "RheelEngine/UI/Elements/ColorElement.h"
#include "RheelEngine/UI/Elements/CrosshairElement.h"
#include "RheelEngine/UI/Elements/EmptyElement.h"
#include "RheelEngine/UI/Elements/SceneElement.h"
#include "RheelEngine/UI/Elements/TextElement.h"
#include "RheelEngine/UI/Elements/VignetteElement.h"
// END GENERATED INCLUDES

// includes for entry point
#include <type_traits>

#include <AL/al.h>

// force dedicated gpu usage
extern "C"
{
__declspec(dllexport) inline unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) inline int AmdPowerXpressRequestHighPerformance = 1;
}

// entry point
namespace rheel {

template<typename GameClass>
void entry() {
	static_assert(std::is_default_constructible<GameClass>::value, "Game Class must be default-constructible");
	static_assert(std::is_base_of<rheel::Game, GameClass>::value, "Game Class must derive from rheel::Game");
	Engine::Initialize_();
	Game* game = new GameClass;
	Engine::Run_(game);
	delete game;
	Engine::Terminate_();
}

}

// macro for entry point
#define RHEEL_ENGINE_ENTRY(GameClass)       \
	int main(int argc, char* argv[]) {      \
		rheel::entry<GameClass>();			\
	}

#endif
