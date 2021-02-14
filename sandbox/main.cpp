#include <RheelEngine.h>

using namespace rheel;

class FpsUpdater : public Component<rheel::user_component + 0> {

public:
	void SetElement(TextElement* element) {
		_element = element;
	}

	void Update() override {
		if (_element) {
			_element->text = std::to_string(int(std::round(1.0 / dt))) + " FPS";
		}
	}

private:
	TextElement* _element = nullptr;

};

static void create_cube(Entity& cube, Game& game) {
	static auto box_model = StaticModelGeneratorBox({ 1.0f, 1.0f, 1.0f })();
	auto box_shader = game.GetAssetLoader().glsl.Load("Resources/test_shader.glsl");

	cube.AddComponent<ModelRenderComponent>(box_model, Material({ 0.0f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));

	CosineInterpolator<float> interpolator_1;
	interpolator_1.AddPoint(0.0f, 0.2f);
	interpolator_1.AddPoint(1.0f, 1.0f);
	interpolator_1.AddPoint(2.0f, 0.2f);

	LinearInterpolator<float> interpolator_2;
	interpolator_2.AddPoint(0.0f, 0.0f);
	interpolator_2.AddPoint(2.0f, M_PI * 2.0f);

	auto& animator = cube.AddComponent<AnimatorComponent>();
	Clip clip;

	clip.AddSequence<float>([&cube](float f) {
		cube.GetComponent<ModelRenderComponent>()->SetMaterial(Material({ f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));
		cube.transform.SetTranslation({ 0.0f, f + 0.5f, 0.0f });
	}, std::move(interpolator_1));

	clip.AddSequence<float>([&cube](float f) {
		cube.transform.SetRotation(vec3{ 0.0f, f, 0.0f });
	}, std::move(interpolator_2));

	animator.AddClip("Animation", std::move(clip));
	animator.Loop("Animation");
}

static void create_ramp(Entity& ramp) {
	static auto ramp_model = StaticModelGeneratorBox({ 8.0f, 1.0f, 10.0f })();

	ramp.AddComponent<ModelRenderComponent>(ramp_model, Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));
	ramp.AddComponent<RigidBody>(PhysicsShape::Box({ 4.0f, 0.5f, 5.0f }));
}

static void create_floor(Entity& floor) {
	static auto floor_model = StaticModelGeneratorBox({ 40.0f, 1.0f, 40.0f })();

	floor.AddComponent<ModelRenderComponent>(floor_model, Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));
	floor.AddComponent<RigidBody>(PhysicsShape::Box({ 20.0f, 0.5f, 20.0f }));
}

static ScenePointer create_scene(Game& game) {
	auto scene = game.CreateScene();
	scene->AddRootComponent<FpsUpdater>();

	auto& physics_scene = scene->AddRootComponent<PhysicsScene>();
	physics_scene.SetGravity({ 0.0f, -9.81f, 0.0f });

	auto& cube = scene->AddEntity(Transform({ 0.0f, 0.5f, 0.0f }));
	create_cube(cube, game);

	auto& ramp_1 = scene->AddEntity("ramp1"_id, Transform({ -8, 3, 0 }, quat(vec3{ 0, 0, -0.6f })));
	create_ramp(ramp_1);

	auto& ramp_2 = scene->AddEntity("ramp2"_id, Transform({ 8, 2, 0 }, quat(vec3{ 0, 0, 0.8f })));
	create_ramp(ramp_2);

	auto& floor = scene->AddEntity("floor"_id, Transform({ 0, -0.5f, 0 }));
	create_floor(floor);

	auto& light = scene->AddEntity("main_light"_id);
	auto& light_component = light.AddComponent<DirectionalLight>(Color(1.0f, 1.0f, 1.0f, 1.0f), vec3{ 0.2f, -2.0f, -1.0f });
	light_component.SetShadowDistance(100.0f);

	auto& camera = scene->AddEntity("main_camera"_id, Transform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
	camera.AddComponent<PerspectiveCamera>(75.0f, 0.01f, 100.0f);
	camera.AddComponent<EulerController>();

	return scene;
}

class SandboxGame : public Game {

public:
	SandboxGame() :
			Game(GetDisplayConfiguration_(), "Sandbox") {}

	void Start() override {
		SetActiveScene(create_scene(*this));

		Container ui;

		auto* scene_element = ui.InsertElement(SceneElement("main_camera"_id));
		ui.AddConstraint(scene_element, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(scene_element, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);
		scene_element->SetGrabOnFocus(false);

		auto *vignette_element = ui.InsertElement(VignetteElement(Color{ 0.0f, 0.0f, 0.0f, 0.5f }, 0.5f, 2.0f));
		ui.AddConstraint(vignette_element, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(vignette_element, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		auto* crosshair_element = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshair_element, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshair_element, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshair_element, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshair_element, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		auto* fps_element = ui.InsertElement(TextElement("0 FPS", 20, 0xFFFFFF));
		ui.AddConstraint(fps_element, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		auto* button_element = ui.InsertElement(ButtonElement([this]() { Stop(); }));
		ui.AddConstraint(button_element, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT, 100);
		ui.AddConstraint(button_element, Constraint::TOP_LEFT, button_element, Constraint::BOTTOM_RIGHT, -100);

		GetUI().SetContainer(std::move(ui));
		GetActiveScene()->GetRootComponent<FpsUpdater>()->SetElement(fps_element);
	}

	static DisplayConfiguration GetDisplayConfiguration_() {
		DisplayConfiguration config;
		config.window_mode = DisplayConfiguration::WINDOWED_UNRESIZABLE;
		config.shadow_quality = DisplayConfiguration::SHADOW_HIGH;
		config.aa_mode = DisplayConfiguration::AntiAliasing::MSAA_4;
		config.vsync = true;
		return config;
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
