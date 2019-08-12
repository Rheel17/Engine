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
		Container ui;
		ColorElement white({ 1, 1, 1, 1 });
		ColorElement red({ 1, 0, 0, 1 });
		ColorElement green({ 0, 1, 0, 1 });
		ColorElement black({ 0, 0, 0, 1 });

		// background: white
		Element *whitePtr = ui.AddElement(white);
		ui.AddConstraint(whitePtr, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 0);
		ui.AddConstraint(whitePtr, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT, 0);

		// black square
		Element *blackPtr = ui.AddElement(black);
		ui.AddHeightRelativeConstraint(blackPtr, Constraint::TOP, nullptr, Constraint::TOP, 0.4f);
		ui.AddHeightRelativeConstraint(blackPtr, Constraint::BOTTOM, nullptr, Constraint::BOTTOM, -0.4f);
		ui.AddHeightRelativeConstraint(blackPtr, Constraint::LEFT, nullptr, Constraint::LEFT, 0.4f);
		ui.AddHeightRelativeConstraint(blackPtr, Constraint::RIGHT, blackPtr, Constraint::LEFT, 0.2f);

		Container subUI;

		Element *redPtr = subUI.AddElement(red);
		subUI.AddConstraint(redPtr, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		subUI.AddConstraint(redPtr, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);
		subUI.AddWidthRelativeConstraint(redPtr, Constraint::RIGHT, redPtr, Constraint::LEFT, 0.5F);

		Element *greenPtr = subUI.AddElement(green);
		subUI.AddConstraint(greenPtr, Constraint::TOP_LEFT, redPtr, Constraint::TOP_RIGHT);
		subUI.AddConstraint(greenPtr, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT, -10);

		Element *subUIPtr = ui.InsertElement(std::move(subUI));
		ui.AddConstraint(subUIPtr, Constraint::TOP, blackPtr, Constraint::TOP);
		ui.AddHeightRelativeConstraint(subUIPtr, Constraint::LEFT, blackPtr, Constraint::RIGHT, 0.2f);
		ui.AddHeightRelativeConstraint(subUIPtr, Constraint::RIGHT, subUIPtr, Constraint::LEFT, 0.3f);
		ui.AddConstraint(subUIPtr, Constraint::BOTTOM, blackPtr, Constraint::BOTTOM);

		Engine::GetUI().SetContainer(std::move(ui));
	}

private:

};

RHEEL_ENGINE_ENTRY(SandboxGame)
