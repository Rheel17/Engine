#include <RheelEngine.h>

using namespace rheel;
using namespace rheel::ui;

class RotationComponent : public rheel::Component {

public:
	void OnUpdate() override {
		Parent().Rotate(quat(0.9999996f, 0.0f, 0.0008727f, 0.0f));
	}

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("object");

	ModelPtr model = Model::LoadCollada("sphere.dae");

	blueprint.AddComponent("rotation");

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
		component->SetMaterial(Material::UV_TEST_MATERIAL);
//		component->SetMaterial(Material(Color{ 1.0f, 0.4f, 0.1f, 1.0f }, 0.9f, 1.0f, 50.0f));
	});

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddObject("object");
	description.AddDirectionalLight("main_light", { 1, 1, 1, 1 }, { -1, 0, 0 });
	description.AddSpotLight("green_light", { -5, 0, 0 }, { 0, 1, 0, 1 }, { 1, 0, 0 }, 50.0f, 0.1f);
	description.AddCamera(75.0f, 0.01f, 1000.0f, "main_camera", { 0, 0, -3 });

	return description;
}

class SandboxGame : public Game {
	void RegisterComponents() override {
		Engine::RegisterComponent<RotationComponent>("rotation");
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
