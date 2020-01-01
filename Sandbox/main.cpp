/*
 * Copyright � 2019 Levi van Rheenen. All rights reserved.
 */
#include <RheelEngine.h>

using namespace rheel;

class FpsUpdater : public ComponentBase {

public:
	void SetElement(TextElement *element) {
		_element = element;
	}

	void Update() override {
		_element->SetText(std::to_string(int(std::round(1.0 / GetTimeDelta()))) + " FPS");
	}

private:
	TextElement *_element = nullptr;

};

static void createCube(Entity *cube) {
	cube->AddComponent<ModelRenderComponent>(
			ModelResource::Box({ 0.5f, 0.5f, 0.5f }),
			Material(ResourceManager::GetImage("uv_grid.png"), 0.7f, 0.5f));

	cube->AddComponent<RigidBody>(
			PhysicsShape::Box({ 0.5f, 0.5f, 0.5f }), 5.0f, 0.05f);
}

static void createRamp(Entity *ramp) {
	ramp->AddComponent<ModelRenderComponent>(
			ModelResource::Box({ 4.0f, 0.5f, 5.0f }),
			Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));

	ramp->AddComponent<RigidBody>(
			PhysicsShape::Box({ 4.0f, 0.5f, 5.0f }));
}

static void createFloor(Entity *ramp) {
	ramp->AddComponent<ModelRenderComponent>(
			ModelResource::Box({ 20.0f, 0.5f, 20.0f }),
			Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));

	ramp->AddComponent<RigidBody>(
			PhysicsShape::Box({ 20.0f, 0.5f, 20.0f }));
}

static Scene *createScene() {
	Scene *scene = new Scene();

	scene->AddRootComponent<FpsUpdater>();

	auto physicsScene = scene->AddRootComponent<PhysicsScene>();
	physicsScene->SetGravity({ 0.0f, -9.81f, 0.0f });

	for (int i = -2; i <= 2; i++) {
		for (int j = 0; j < 5; j++) {
			Entity *cube = scene->AddEntity(
					scene->UniqueEntityName("cube"),
					Transform({ 1.1f * i, 1.1f * j + 0.5f, 1.1f * i }));
			createCube(cube);
		}
	}

	Entity *ramp1 = scene->AddEntity("ramp1", Transform({ -8, 3, 0 }, quat(vec3{ 0, 0, -0.6f })));
	createRamp(ramp1);

	Entity *ramp2 = scene->AddEntity("ramp2", Transform({ 8, 2, 0 }, quat(vec3{ 0, 0, 0.8f })));
	createRamp(ramp2);

	Entity *floor = scene->AddEntity("floor", Transform({ 0, -0.5f, 0 }));
	createFloor(floor);

	Entity *light = scene->AddEntity("main_light");
	auto lightComponent = light->AddComponent<DirectionalLight>(Color{ 1, 1, 1, 1 }, vec3{ 0.2f, -2.0f, -1.0f });
	lightComponent->SetShadowDistance(100.0f);

	Entity *camera = scene->AddEntity("main_camera", RigidTransform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
	camera->AddComponent<PerspectiveCamera>("main_camera", 75.0f, 0.01f, 100.0f);
	camera->AddComponent<EulerController>();

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

		TextElement *fpsElement = ui.InsertElement(TextElement("0 FPS", Font::GetDefaultFont(), 20));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		Engine::GetUI().SetContainer(std::move(ui));
		sceneElement->RequestFocus();

		Engine::GetActiveScene()->GetRootComponent<FpsUpdater>()->SetElement(fpsElement);
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
