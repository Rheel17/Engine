#include <RheelEngine.h>

using namespace rheel;

static Blueprint createObjectBlueprint() {
	Blueprint blueprint("object");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetModel(model);

	blueprint.AddComponent<RigidBodyComponent>();

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddScript<PhysicsScene>();

	auto& eulerCameraController = description.AddScript<EulerCameraController>();
	eulerCameraController.SetCamera("main_camera");

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			for (int k = 0; k < 5; k++) {
				auto& obj = description.AddObject("object", { 4 * i, 4 * k, 4 * j });
				obj.GetComponent<ModelRenderComponent>()->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.0f));
				obj.GetComponent<RigidBodyComponent>()->SetShape(PhysicsShape::Box({ 1.0f, 1.0f, 1.0f }));
				obj.GetComponent<RigidBodyComponent>()->SetMass(100.0f);
			}
		}
	}

	auto& floor = description.AddObject("object", { 0, -2, 0 }, { 1, 0, 0, 0 });
	floor.GetComponent<ModelRenderComponent>()->SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.0f));
	floor.GetComponent<ModelRenderComponent>()->SetScale({ 20.0f, 1.0f, 20.0f });
	floor.GetComponent<RigidBodyComponent>()->SetShape(PhysicsShape::Box({ 20.0f, 1.0f, 20.0f }));
	floor.GetComponent<RigidBodyComponent>()->SetMass(0.0f);

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { 0, 2, 12 });

	return description;
}

class SandboxGame : public Game {
	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createObjectBlueprint());
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
