#include <RheelEngine.h>

using namespace rheel;

class FpsUpdater : public ComponentBase {

public:
	void SetElement(TextElement *element) {
		_element = element;
	}

	void Update() override {
		if (_element) {
			_element->SetText(std::to_string(int(std::round(1.0 / GetTimeDelta()))) + " FPS");
		}
	}

private:
	TextElement *_element = nullptr;

};

static void createCube(Entity *cube) {
	static auto boxModel = StaticModelGeneratorBox({ 1.0f, 1.0f, 1.0f })();
	static auto boxShader = Engine::GetAssetLoader().glsl.Load("Resources/test_shader.glsl");

	//cube->AddComponent<ModelRenderComponent>(boxModel, Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.5f));
	cube->AddComponent<RigidBody>(PhysicsShape::Box({ 0.5f, 0.5f, 0.5f }), 5.0f, 0.05f);

	cube->AddComponent<ModelRenderComponent>(boxModel, Material(boxShader));
}

static void createRamp(Entity *ramp) {
	static auto rampModel = StaticModelGeneratorBox({ 8.0f, 1.0f, 10.0f })();

	ramp->AddComponent<ModelRenderComponent>(rampModel, Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));
	ramp->AddComponent<RigidBody>(PhysicsShape::Box({ 4.0f, 0.5f, 5.0f }));
}

static void createFloor(Entity *floor) {
	static auto floorModel = StaticModelGeneratorBox({ 40.0f, 1.0f, 40.0f })();

	floor->AddComponent<ModelRenderComponent>(floorModel, Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));
	floor->AddComponent<RigidBody>(PhysicsShape::Box({ 20.0f, 0.5f, 20.0f }));
}

static Scene *createScene() {
	auto scene = new Scene();
	scene->AddRootComponent<FpsUpdater>();

	auto physicsScene = scene->AddRootComponent<PhysicsScene>();
	physicsScene->SetGravity({ 0.0f, -9.81f, 0.0f });

	for (int i = -2; i <= 2; i++) {
		for (int j = 0; j < 5; j++) {
			Entity *cube = scene->AddEntity(
			scene->UniqueEntityName("cube"),
			RigidTransform({ 1.1f * i, 1.1f * j + 0.5f, 1.1f * i }));
			createCube(cube);
		}
	}

	auto ramp1 = scene->AddEntity("ramp1", RigidTransform({ -8, 3, 0 }, quat(vec3{ 0, 0, -0.6f })));
	createRamp(ramp1);

	auto ramp2 = scene->AddEntity("ramp2", RigidTransform({ 8, 2, 0 }, quat(vec3{ 0, 0, 0.8f })));
	createRamp(ramp2);

	auto floor = scene->AddEntity("floor", RigidTransform({ 0, -0.5f, 0 }));
	createFloor(floor);

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

		auto sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

//		auto vignetteElement = ui.InsertElement(VignetteElement());
//		ui.AddConstraint(vignetteElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
//		ui.AddConstraint(vignetteElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		auto crosshairElement = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		auto fpsElement = ui.InsertElement(TextElement("0 FPS", Font::GetDefaultFont(), 20));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		Engine::GetUI().SetContainer(std::move(ui));
		sceneElement->RequestFocus();

		Engine::GetActiveScene()->GetRootComponent<FpsUpdater>()->SetElement(fpsElement);
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)