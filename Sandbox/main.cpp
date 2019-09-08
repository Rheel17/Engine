#include <RheelEngine.h>

using namespace rheel;

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelPtr model = Model::LoadCollada("cube.dae");

	blueprint.AddComponent(Component::NAME_RIGIDBODY, [](ComponentPtr c) {
		RigidbodyComponent *component = static_cast<RigidbodyComponent *>(c.get());
		component->SetShape(nullptr);
	});

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
	});

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddScript(Script::NAME_PHYSICS_WORLD);
	description.AddScript(Script::NAME_EULER_CAMERA_CONTROLLER, [](ScriptPtr s) {
		EulerCameraController *script = static_cast<EulerCameraController *>(s.get());
		script->SetCamera("main_camera");
	});

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			for (int k = 0; k < 5; k++) {
				auto& cube = description.AddObject("cube", { 4 * i, 4 * k, 4 * j });
				cube.loader = [](ObjectPtr object) {
					auto renderer = object->GetComponent<ModelRenderComponent>();
					renderer->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.0f));
				};
			}
		}
	}

	auto& floor = description.AddObject("cube", { 0, -2, 0 }, quat(), { 20, 1, 20 });
	floor.loader = [](ObjectPtr object) {
		auto renderer = object->GetComponent<ModelRenderComponent>();
		renderer->SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.0f));
	};

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { 0, 2, 12 });

	return description;
}

class SandboxGame : public Game {
	void RegisterComponents() override {}

	void RegisterScripts() override {}

	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createCubeBlueprint());
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
		config.vsync = false;

		Engine::SetDisplayConfiguration(std::move(config));
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
