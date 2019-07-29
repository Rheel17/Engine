#include <RheelEngine.h>

using namespace rheel;

class RotationComponent : public rheel::Component {

public:
	void OnUpdate() override {
		Parent().Rotate(quat(0.9999996f, 0.0f, 0.0008727f, 0.0f));
	}

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("object");

	ModelPtr model = Model::LoadCollada("suzanne_hires.dae");

	blueprint.AddComponent("rotation");

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
		component->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.9f, 1.0f));
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
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;

		Engine::SetDisplayConfiguration(std::move(config));
	}

	void Start() override {
		Engine::SetActiveScene("main");

		UIPtr ui = UI::Create();

		ElementPtr sceneView = SceneElement::Create("main_camera");
		ui->GetContainer()->AddElement(sceneView);

		ElementPtr textView = TextElement::Create(L"Text element!", 20);
		ui->GetContainer()->AddElement(textView);

		ui->GetContainer()->AddConstraint(sceneView, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui->GetContainer()->AddConstraint(sceneView, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);
		ui->GetContainer()->AddConstraint(textView, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 50);

		Engine::SetUI(ui);
	}
};

RHEEL_ENGINE_ENTRY(SandboxGame)
