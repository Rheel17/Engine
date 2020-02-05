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

static void createVoxel(Entity *vox) {
	VoxelImage model = Engine::GetAssetLoader().voxel.Load("Resources/monu10.vox");
}

static Scene *createScene() {
	auto scene = new Scene();
	scene->AddRootComponent<FpsUpdater>();

	auto entity = scene->AddEntity("VoxelEntity");
	createVoxel(entity);

	auto light = scene->AddEntity("main_light");
	auto lightComponent = light->AddComponent<DirectionalLight>(Color{ 1, 1, 1, 1 }, vec3{ 0.2f, -2.0f, -1.0f });
	lightComponent->SetShadowDistance(100.0f);

	auto camera = scene->AddEntity("main_camera", RigidTransform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
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
