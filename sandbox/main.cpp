#include <RheelEngine.h>

using namespace rheel;

class FpsUpdater : public ComponentBase {

public:
	void SetElement(TextElement* element) {
		_element = element;
		_element->text = "";
	}

	void Update() override {
		if (_element) {
			// _element->text = std::to_string(int(std::round(1.0 / GetTimeDelta()))) + " FPS";
		}
	}

private:
	TextElement* _element = nullptr;

};

class LoremElement : public Element {

protected:
	virtual void DoDraw(float time, float dt) const override {
		GetTextRenderer().DrawParagraph(
				"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent porttitor arcu ut velit faucibus, eget tristique turpis volutpat. Quisque semper arcu ante, ut vestibulum leo ultrices et. Donec fringilla dictum sodales. Nullam semper mauris vel tristique vestibulum. Sed pulvinar aliquet commodo. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Fusce ornare augue et odio maximus viverra. Proin auctor in elit quis ultricies. Fusce auctor libero est, nec laoreet erat euismod iaculis.\n"
				"\n"
				"Pellentesque sagittis facilisis varius. Suspendisse in ornare augue. Etiam dictum vulputate velit in accumsan. Donec eget efficitur nulla. Etiam a scelerisque tellus. Fusce eget massa lectus. Donec facilisis scelerisque nisl, sit amet tristique est fermentum at. Pellentesque non nulla nec lorem blandit iaculis nec nec velit. Praesent ut malesuada ipsum. In hac habitasse platea dictumst. Nulla nec eros vulputate, sollicitudin neque euismod, tincidunt massa. Sed sapien nulla, ornare vel mauris sed, facilisis placerat magna.\n"
				"\n"
				"Nam et justo eu ante maximus tincidunt tempor non enim. Vestibulum quis facilisis metus. Etiam vitae velit magna. Vestibulum sit amet facilisis eros, in maximus elit. Phasellus a ipsum in lorem sagittis interdum nec at ante. Donec enim leo, feugiat lobortis mattis sit amet, hendrerit at lacus. Donec id ipsum id nisl feugiat sollicitudin. Ut at eros faucibus, condimentum nisl non, euismod ipsum. Vivamus efficitur mollis diam non eleifend. Maecenas non odio sed felis lacinia dignissim. Nullam malesuada suscipit sapien ut auctor. In dui ipsum, semper ut orci ut, luctus interdum metus. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Pellentesque luctus dolor quis turpis dapibus pretium.\n"
				"\n"
				"Nam iaculis consequat dui, id ultrices ex elementum et. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Aliquam fringilla metus eu iaculis mattis. Fusce venenatis purus diam, nec tincidunt erat volutpat non. Mauris sit amet urna bibendum, tincidunt ligula sed, pharetra ipsum. Integer rutrum, purus a viverra dictum, urna enim aliquet dui, sit amet accumsan tortor orci in nisl. Pellentesque id nibh odio. Etiam nec est sed enim porttitor congue. Mauris fringilla nec dui et tincidunt. Fusce augue mi, porta sed odio et, aliquam venenatis dui. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Integer maximus pharetra quam, id efficitur dui tincidunt eget.\n"
				"\n"
				"Duis pellentesque odio in turpis mollis aliquet. Nam mollis ipsum et eros hendrerit, et sodales mi viverra. Ut iaculis velit quis ligula accumsan, ut mollis nisl luctus. Curabitur non lorem non dolor vehicula iaculis eu et lectus. Nullam vel interdum erat. Pellentesque vel elit porttitor, congue neque ut, convallis felis. Maecenas posuere justo id diam faucibus, eget eleifend augue placerat.",
				100, 40, GetBounds().width - 200,
				Font::GetDefaultFont(), 20,
				0xFFFFFF,
				TextAlign::RIGHT);
	}

};

static void createCube(Entity* cube, Game& game) {
	static auto boxModel = StaticModelGeneratorBox({ 1.0f, 1.0f, 1.0f })();
	auto boxShader = game.GetAssetLoader().glsl.Load("Resources/test_shader.glsl");

	cube->AddComponent<ModelRenderComponent>(boxModel, Material({ 0.0f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));

	CosineInterpolator<float> interpolator1;
	interpolator1.AddPoint(0.0f, 0.2f);
	interpolator1.AddPoint(1.0f, 1.0f);
	interpolator1.AddPoint(2.0f, 0.2f);

	LinearInterpolator<float> interpolator2;
	interpolator2.AddPoint(0.0f, 0.0f);
	interpolator2.AddPoint(2.0f, M_PI * 2.0f);

	auto animator = cube->AddComponent<AnimatorComponent>();
	Clip clip;

	clip.AddSequence<float>([cube](float f) {
		auto modelRenderer = cube->GetComponent<ModelRenderComponent>();
		modelRenderer->SetMaterial(Material({ f, 0.2f, 0.2f, 1.0f }, 0.7f, 0.5f));

		cube->transform.SetTranslation({ 0.0f, f + 0.5f, 0.0f });
	}, std::move(interpolator1));

	clip.AddSequence<float>([cube](float f) {
		cube->transform.SetRotation(vec3{ 0.0f, f, 0.0f });
	}, std::move(interpolator2));

	animator->AddClip("Animation", std::move(clip));
	animator->Loop("Animation");
}

static void createRamp(Entity* ramp) {
	static auto rampModel = StaticModelGeneratorBox({ 8.0f, 1.0f, 10.0f })();

	ramp->AddComponent<ModelRenderComponent>(rampModel, Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));
	ramp->AddComponent<RigidBody>(PhysicsShape::Box({ 4.0f, 0.5f, 5.0f }));
}

static void createFloor(Entity* floor) {
	static auto floorModel = StaticModelGeneratorBox({ 40.0f, 1.0f, 40.0f })();

	floor->AddComponent<ModelRenderComponent>(floorModel, Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));
	floor->AddComponent<RigidBody>(PhysicsShape::Box({ 20.0f, 0.5f, 20.0f }));
}

static ScenePointer createScene(Game& game) {
	auto scene = game.CreateScene();
	scene->AddRootComponent<FpsUpdater>();

	auto physicsScene = scene->AddRootComponent<PhysicsScene>();
	physicsScene->SetGravity({ 0.0f, -9.81f, 0.0f });

	auto cube = scene->AddEntity(
			scene->UniqueEntityName("cube"),
			RigidTransform({ 0.0f, 0.5f, 0.0f }));
	createCube(cube, game);

	auto ramp1 = scene->AddEntity("ramp1", RigidTransform({ -8, 3, 0 }, quat(vec3{ 0, 0, -0.6f })));
	createRamp(ramp1);

	auto ramp2 = scene->AddEntity("ramp2", RigidTransform({ 8, 2, 0 }, quat(vec3{ 0, 0, 0.8f })));
	createRamp(ramp2);

	auto floor = scene->AddEntity("floor", RigidTransform({ 0, -0.5f, 0 }));
	createFloor(floor);

	auto light = scene->AddEntity("main_light");
	auto lightComponent = light->AddComponent<DirectionalLight>(Color(1.0f, 1.0f, 1.0f, 1.0f), vec3{ 0.2f, -2.0f, -1.0f });
	lightComponent->SetShadowDistance(100.0f);

	auto camera = scene->AddEntity("main_camera", RigidTransform(vec3{ -12.0f, 7.5f, 0.0f }, vec3{ 0.0f, -M_PI / 2.0f, 0.0f }));
	camera->AddComponent<PerspectiveCamera>("main_camera", 75.0f, 0.01f, 100.0f);
	camera->AddComponent<EulerController>();

	return scene;
}

class SandboxGame : public Game {

public:
	SandboxGame() :
			Game(GetDisplayConfiguration_(), "Sandbox") {}

	void Start() override {
		SetActiveScene(createScene(*this));

		Container ui;

		auto sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);
		sceneElement->SetGrabOnFocus(false);

		auto vignetteElement = ui.InsertElement(VignetteElement(Color{ 0.0f, 0.0f, 0.0f, 0.5f }, 0.5f, 2.0f));
		ui.AddConstraint(vignetteElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(vignetteElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		auto crosshairElement = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		auto fpsElement = ui.InsertElement(TextElement("0 FPS", 20, 0xFFFFFF));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		auto buttonElement = ui.InsertElement(ButtonElement([this](){ Stop(); }));
		ui.AddConstraint(buttonElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT,  100);
		ui.AddConstraint(buttonElement, Constraint::TOP_LEFT, buttonElement, Constraint::BOTTOM_RIGHT, -100);

		auto loremElement = ui.InsertElement(LoremElement());
		ui.AddConstraint(loremElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(loremElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);

		GetUI().SetContainer(std::move(ui));
		GetActiveScene()->GetRootComponent<FpsUpdater>()->SetElement(fpsElement);
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