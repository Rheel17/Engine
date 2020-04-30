/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_RHEELENGINE_H
#define RHEELENGINE_RHEELENGINE_H

// includes for all required header files
// BEGIN GENERATED INCLUDES
#include "RheelEngine/Game.h"
#include "RheelEngine/Scene.h"
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
#include "RheelEngine/UI/UI.h"
#include "RheelEngine/UI/Elements/ColorElement.h"
#include "RheelEngine/UI/Elements/CrosshairElement.h"
#include "RheelEngine/UI/Elements/EmptyElement.h"
#include "RheelEngine/UI/Elements/SceneElement.h"
#include "RheelEngine/UI/Elements/TextElement.h"
#include "RheelEngine/UI/Elements/VignetteElement.h"
// END GENERATED INCLUDES

// includes for entry point
#include <type_traits>

// entry point
namespace rheel {

template<typename GameClass>
void entry() {
	static_assert(std::is_default_constructible<GameClass>::value, "Game Class must be default-constructible");
	static_assert(std::is_base_of<rheel::Game, GameClass>::value, "Game Class must derive from rheel::Game");

	Game* game = new GameClass;
	game->Start();
	game->Loop_();
	delete game;
}

}

// macro for entry point
#define RHEEL_ENGINE_ENTRY(GameClass)	      									\
	/* Force dedicated GPU usage */												\
	extern "C" {																\
		__declspec(dllexport) unsigned long NvOptimusEnablement = 1;			\
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;		\
	}																			\
																				\
	int main(int argc, char* argv[]) {      									\
		rheel::entry<GameClass>();												\
	}

#endif
