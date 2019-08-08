#include <RheelEngine.h>

using namespace rheel;

class RandomRemoveScript : public Script {

public:
	void PreOnUpdate() override {
		_timer++;
		_timer %= 60;

		if (_timer == 0 && !_objects.empty()) {
			unsigned index = rand() % _objects.size();
			ObjectPtr object = _objects[index];
			_objects.erase(_objects.begin() + index);

			Parent().RemoveObject(object);
		}
	}

	void AddObject(ObjectPtr object) {
		_objects.push_back(object);
	}

private:
	std::vector<ObjectPtr> _objects;
	unsigned _timer;

};

class RandomRemoveComponent : public Component {

public:
	void OnAdd() override {
		Parent()->ParentScene()->GetScript<RandomRemoveScript>()->AddObject(Parent());
	}

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelPtr model = Model::LoadCollada("cube.dae");

	blueprint.AddComponent(Component::NAME_MODELRENDER, [model](ComponentPtr c) {
		ModelRenderComponent *component = static_cast<ModelRenderComponent *>(c.get());
		component->SetModel(model);
		component->SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.9f, 1.0f));
	});

	blueprint.AddComponent("RandomRemove");

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");
	description.AddScript("RandomRemove");

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			description.AddObject("cube", { 4 * i, 0, 4 * j });
		}
	}

	description.AddDirectionalLight("main_light", { 1, 1, 1, 1 }, { 0, -1, -1 });
	description.AddCamera("main_camera", 75.0f, 0.01f, 1000.0f, { 0, 12, -10 }, { 0.8660254f, 0.5f, 0, 0 });

	return description;
}

class SandboxGame : public Game {
	void RegisterComponents() override {
		Engine::RegisterComponent<RandomRemoveComponent>("RandomRemove");
	}

	void RegisterScripts() override {
		Engine::RegisterScript<RandomRemoveScript>("RandomRemove");
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

		SceneElement *sceneView = new SceneElement("main_camera");
		ui->GetContainer()->AddElement(sceneView);

		ui->GetContainer()->AddConstraint(sceneView, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui->GetContainer()->AddConstraint(sceneView, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		Engine::SetUI(ui);
	}
};

RHEEL_ENGINE_ENTRY(SandboxGame)
