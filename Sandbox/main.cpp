/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include <RheelEngine.h>

using namespace rheel;

static void createCube(Entity *cube) {
	ModelResource& model = ModelResource::Box({ 0.5f, 0.5f, 0.5f });

	cube->AddComponent<ModelRenderComponent>(
			model,												// model
			Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.5f)	// material
	);

	cube->AddComponent<RigidBodyComponent>(
			PhysicsShape::Box({ 0.5f, 0.5f, 0.5f }),	// shape
			5.0f,										// mass
			0.05f										// bounciness
	);
}

static Scene *createScene() {
	Scene *scene = new Scene();

	scene->AddRootComponent<EulerController>();

//	auto physicsScene = scene->AddRootComponent<PhysicsScene>();
//	physicsScene->SetGravity({ 0.0f, -9.81f, 0.0f });

//	for (int i = -2; i <= 2; i++) {
//		for (int j = 0; j < 5; j++) {
//			Entity *cube = scene->AddEntity(
//					scene->UniqueEntityName("cube"),
//					Transform({ 1.1f * i, 1.1f * j + 0.5f, 1.1f * i }));
//			createCube(cube);
//		}
//	}

	Entity *cube = scene->AddEntity("cube", Transform(vec3{ 0, 0, 0 }));
	createCube(cube);

	Entity *light = scene->AddEntity("main_light");
	auto lightComponent = light->AddComponent<DirectionalLight>(Color{ 1, 1, 1, 1 }, vec3{ 0.2f, -2.0f, -1.0f });
	lightComponent->SetShadowDistance(100.0f);

	Entity *camera = scene->AddEntity("main_camera", RigidTransform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
	camera->AddComponent<PerspectiveCamera>("main_camera", 75.0f, 0.01f, 100.0f);

	return scene;
}

class SandboxGame : public Game {
	void Initialize() override {
		DisplayConfiguration config;
		config.title = "Sandbox";
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;
		config.shadow_quality = DisplayConfiguration::SHADOW_HIGH;
		config.aa_mode = DisplayConfiguration::AntiAliasing::MSAA_4;
		config.vsync = true;

		Engine::SetDisplayConfiguration(std::move(config));

		ResourceManager::SetResourcePath("Resources");
	}

	void Start() override {
		Engine::SetActiveScene(createScene());

		Container ui;

		SceneElement *sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		CrosshairElement *crosshairElement = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		Engine::GetUI().SetContainer(std::move(ui));
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
