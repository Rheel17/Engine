#include <RheelEngine.h>

using namespace rheel;

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 1.0f, 1.0f, 1.0f }));
	rigidBodyComponent.SetMass(1000.0f);

	return blueprint;
}

static Blueprint createFloorBlueprint() {
	Blueprint blueprint("floor");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetScale({ 200.0f, 1.0f, 200.0f });
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 200.0f, 1.0f, 200.0f }));

	return blueprint;
}

static Blueprint createBallBlueprint() {
	Blueprint blueprint("ball");

	ModelResource& model = ResourceManager::GetModel("sphere.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 1.0f, 1.0f, 1.0f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Sphere(1.0f));
	rigidBodyComponent.SetMass(2500.0f);

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	auto& physicsScene = description.AddScript<PhysicsScene>();
	physicsScene.SetGravity({ 0.0f, -9.81f, 0.0f });

	auto& eulerCameraController = description.AddScript<EulerCameraController>();
	eulerCameraController.SetCamera("main_camera");

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			for (int k = 0; k < 5; k++) {
				description.AddObject("cube", { 2.1f * i, 2.1f * k, 2.1f * j });
			}
		}
	}

	description.AddObject("floor", { 0, -2, 0 });
	description.AddObject("ball", { -20, 20, 0 });

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { 0, 2, 30 });

	return description;
}

class SandboxGame : public Game {
	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createCubeBlueprint());
		Engine::RegisterBlueprint(createFloorBlueprint());
		Engine::RegisterBlueprint(createBallBlueprint());
	};

	void RegisterSceneDescriptions() override {
		Engine::RegisterSceneDescription(createSceneDescription());
	}

	void Initialize() override {
		DisplayConfiguration config;
		config.title = "Sandbox";
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;
		config.render_mode = DisplayConfiguration::FORWARD;
		config.shadow_quality = DisplayConfiguration::SHADOW_HIGH;
		config.aa_mode = DisplayConfiguration::AntiAliasing::MSAA_4;
		config.vsync = true;

		Engine::SetDisplayConfiguration(std::move(config));

		ResourceManager::SetResourcePath("Resources");
	}

	void Start() override {
		Engine::SetActiveScene("main");

		Container ui;

		SceneElement *sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		Engine::GetUI().SetContainer(std::move(ui));
		Engine::GetUI().RequestFocus(sceneElement);
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
