#include <RheelEngine.h>

using namespace rheel;

class FpsUpdaterScript : public Script {

public:
	void SetElement(TextElement *textElement) {
		_element = textElement;
	}

	void PreOnUpdate() override {

		if (_element) {
			const int freq = 30;

			if (++_frame_counter == freq) {
				_element->SetText(std::to_string(int(1.0f / TimeDelta())) + " FPS");
				_frame_counter %= freq;
			}
		}
	}

private:
	TextElement *_element;
	int _frame_counter;

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelPtr model = Model::LoadCollada("cube.dae");

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
	});

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddScript(Script::NAME_EULER_CAMERA_CONTROLLER, [](ScriptPtr s) {
		EulerCameraController *script = static_cast<EulerCameraController *>(s.get());
		script->SetCamera("main_camera");
	});

	description.AddScript("FpsUpdaterScript");

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			auto& cube = description.AddObject("cube", { 4 * i, 0, 4 * j });
			cube.loader = [](ObjectPtr object) {
				auto renderer = object->GetComponent<ModelRenderComponent>();
				renderer->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.0f));
			};
		}
	}

	auto& floor = description.AddObject("cube", { 0, -2, 0 }, quat(), { 20, 1, 20 });
	floor.loader = [](ObjectPtr object) {
		auto renderer = object->GetComponent<ModelRenderComponent>();
		renderer->SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.0f));
	};

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
//	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { 0, 2, 12 });
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { 0.6708f, 0.2747f, 5.6670f }, { -0.5382f, 2.0107f, 0.0f });

	return description;
}

class SandboxGame : public Game {
	void RegisterComponents() override {}

	void RegisterScripts() override {
		Engine::RegisterScript<FpsUpdaterScript>("FpsUpdaterScript");
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
		config.shadow_quality = DisplayConfiguration::SHADOW_HIGH;
		config.vsync = false;

		Engine::SetDisplayConfiguration(std::move(config));
	}

	void Start() override {
		Engine::SetActiveScene("main");

		Container ui;

		SceneElement *sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		TextElement *fpsElement = ui.InsertElement(TextElement("0 FPS", 16));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		Engine::GetUI().SetContainer(std::move(ui));
		Engine::GetUI().RequestFocus(sceneElement);

		Engine::GetActiveScene()->GetScript<FpsUpdaterScript>()->SetElement(fpsElement);
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
