#include <RheelEngine.h>

using namespace rheel;

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelPtr model = Model::LoadCollada("cube.dae");

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
		component->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.9f, 1.0f));
	});

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			description.AddObject("cube", { -2 * i, 0, -2 * j });
			description.AddObject("cube", {  2 * i, 0,  2 * j });
		}
	}

	description.AddDirectionalLight("main_light", { 1, 1, 1, 1 }, { 0, -1, -1 });
	description.AddCamera("main_camera", 75.0f, 0.01f, 1000.0f, { 0, 10, -5 }, { 0.819152f, 0.5735764f, 0, 0 });

	return description;
}

class SandboxGame : public Game {
	void RegisterComponents() override {

	}

	void RegisterScripts() override {

	}

	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createCubeBlueprint());
	};

	void RegisterSceneDescriptions() override {
		Engine::RegisterSceneDescription(createSceneDescription());
	}

	void Initialize() override {
		DisplayConfiguration config;
		config.title = "Sandbox";
		config.aa_mode = DisplayConfiguration::AntiAliasing::MSAA_4;
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;

		Engine::SetDisplayConfiguration(std::move(config));
	}

	void Start() override {
		Engine::SetActiveScene("main");

		UIPtr ui = UI::Create();

		ElementPtr sceneView = SceneElement::Create("main_camera");
		ui->GetContainer()->AddElement(sceneView);

		ui->GetContainer()->AddConstraint(sceneView, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui->GetContainer()->AddConstraint(sceneView, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		Engine::SetUI(ui);
	}
};

RHEEL_ENGINE_ENTRY(SandboxGame)
