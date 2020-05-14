#include <RheelEngine.h>
#include <RheelEngine/Renderer/Text/Encoding.h>

using namespace rheel;

class FpsUpdater : public ComponentBase {

public:
	void SetElement(TextElement* element) {
		_element = element;
	}

	void Update() override {
		if (_element) {
			_element->SetText(std::to_string(int(std::round(1.0 / GetTimeDelta()))) + " FPS");
		}
	}

private:
	TextElement* _element = nullptr;

};

class Lorem : public Element {
protected:
	virtual void DoDraw(float time, float dt) const override {
		const static std::string lorem[] = {
				"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam ",
				"felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, ",
				"venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, ",
				"consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi ",
				"vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui.",
				"",
				"Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas ",
				"nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec ",
				"sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis ",
				"sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis ",
				"mi consectetuer lacinia.",
				"",
				"Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. ",
				"Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies ",
				"sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo ",
				"dolor, tempus non, auctor et, hendrerit quis, nisi.",
				"",
				"Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus ",
				"accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis ",
				"orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis.",
				"",
				"Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse ",
				"pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id ",
				"purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.",
				"",
				"In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. ",
				"Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. ",
				"Phasellus magna.",
				"",
				"In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Curabitur a felis in nunc fringilla tristique. Morbi mattis ullamcorper velit. Phasellus gravida semper nisi. Nullam vel sem. ",
				"Pellentesque libero tortor, tincidunt et, tincidunt eget, semper nec, quam. Sed hendrerit. Morbi ac felis. Nunc egestas, augue at pellentesque laoreet, felis eros vehicula leo, at malesuada velit leo quis ",
				"pede. Donec interdum, metus et hendrerit aliquet, dolor diam sagittis ligula, eget egestas libero turpis vel mi. Nunc nulla. Fusce risus nisl, viverra et, tempor et, pretium in, sapien. Donec venenatis ",
				"vulputate lorem.",
				"",
				"Morbi nec metus. Phasellus blandit leo ut odio. Maecenas ullamcorper, dui et placerat feugiat, eros pede varius nisi, condimentum viverra felis nunc et lorem. Sed magna purus, fermentum eu, tincidunt eu, ",
				"varius ut, felis. In auctor lobortis lacus. Quisque libero metus, condimentum nec, tempor a, commodo mollis, magna. Vestibulum ullamcorper mauris at ligula. Fusce fermentum. Nullam cursus lacinia erat. ",
				"Praesent blandit laoreet nibh.",
				"",
				"Fusce convallis metus id felis luctus adipiscing. Pellentesque egestas, neque sit amet convallis pulvinar, justo nulla eleifend augue, ac auctor orci leo non est. Quisque id mi. Ut tincidunt tincidunt ",
				"erat. Etiam feugiat lorem non metus. Vestibulum dapibus nunc ac augue. Curabitur vestibulum aliquam leo. Praesent egestas neque eu enim. In hac habitasse platea dictumst. Fusce a quam. Etiam ut purus ",
				"mattis mauris sodales aliquam. Curabitur nisi. Quisque malesuada placerat nisl. Nam ipsum risus, rutrum vitae, vestibulum eu, molestie vel, lacus.",
				"",
				"Sed augue ipsum, egestas nec, vestibulum et, malesuada adipiscing, dui. Vestibulum facilisis, purus nec pulvinar iaculis, ligula mi congue nunc, vitae euismod ligula urna in dolor. Mauris sollicitudin ",
				"fermentum libero. Praesent nonummy mi in odio. Nunc interdum lacus sit amet orci. Vestibulum rutrum, mi nec elementum vehicula, eros quam gravida nisl, id fringilla neque ante vel mi. Morbi mollis tellus ",
				"ac sapien. Phasellus volutpat, metus eget egestas mollis, lacus lacus blandit dui, id egestas quam mauris ut lacus. Fusce vel dui. Sed in libero ut nibh placerat accumsan. Proin faucibus arcu quis ante. ",
				"In consectetuer turpis ut velit. Nulla sit amet est. Praesent metus tellus, elementum eu, semper a, adipiscing nec, purus. Cras risus ipsum, faucibus ut, ullamcorper id, varius ac, leo. Suspendisse ",
				"feugiat. Suspendisse enim turpis, dictum sed, iaculis a, condimentum nec, nisi. Praesent nec nisl a purus blandit viverra. Praesent ac massa at ligula laoreet iaculis. Nulla neque dolor, sagittis eget, ",
				"iaculis quis, molestie non, velit.",
				"",
				"Mauris turpis nunc, blandit et, volutpat molestie, porta ut, ligula. Fusce pharetra convallis urna. Quisque ut nisi. Donec mi odio, faucibus at, scelerisque quis, convallis in, nisi. Suspendisse non nisl ",
				"sit amet velit hendrerit rutrum. Ut leo. Ut a nisl id ante tempus hendrerit. Proin pretium, leo ac pellentesque mollis, felis nunc ultrices eros, sed gravida augue augue mollis justo. Suspendisse eu ",
				"ligula. Nulla facilisi. Donec id justo. Praesent porttitor, nulla vitae posuere iaculis, arcu nisl dignissim dolor, a pretium mi sem ut ipsum. Curabitur suscipit suscipit tellus.",
				"",
				"Praesent vestibulum dapibus nibh. Etiam iaculis nunc ac metus. Ut id nisl quis enim dignissim sagittis. Etiam sollicitudin, ipsum eu pulvinar rutrum, tellus ipsum laoreet sapien, quis venenatis ante odio ",
				"sit amet eros. Proin magna. Duis vel nibh at velit scelerisque suscipit. Curabitur turpis. Vestibulum suscipit nulla quis orci. Fusce ac felis sit amet ligula pharetra condimentum. Maecenas egestas arcu ",
				"quis ligula mattis placerat. Duis lobortis massa imperdiet quam. Suspendisse potenti."
		};

		int y = 20;
		for (const std::string& line : lorem) {
			GetTextRenderer().DrawText(Font::GetDefaultFont(), 0xFFFFFF, line, 100, y, 14);
			y += 14;
		}
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

		auto fpsElement = ui.InsertElement(TextElement("0 FPS", Font::GetDefaultFont(), 20));
		ui.AddConstraint(fpsElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT, 10);

		auto buttonElement = ui.InsertElement(ButtonElement([this](){ Stop(); }));
		ui.AddConstraint(buttonElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT,  100);
		ui.AddConstraint(buttonElement, Constraint::TOP_LEFT, buttonElement, Constraint::BOTTOM_RIGHT, -100);

		auto loremElement = ui.InsertElement(Lorem());
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