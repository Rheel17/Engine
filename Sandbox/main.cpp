#include <RheelEngine.h>

using namespace rheel;

class GameInputScript : public Script {
	SCRIPT_INIT(GameInputScript);

public:
	void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) override {
		if (button == Input::MouseButton::RIGHT) {
			if (InputSource()->HasFocus()) {
				InputSource()->LoseFocus();
				Parent().GetScript<EulerCameraController>()->SetActive(false);
			} else {
				InputSource()->SetFocusable(true);
				InputSource()->RequestFocus();
				InputSource()->SetFocusable(false);
				Parent().GetScript<EulerCameraController>()->SetActive(true);
			}
		} else if (button == Input::MouseButton::LEFT) {
			if (InputSource()->HasFocus()) {
				auto camera = Parent().GetCamera("main_camera");
				Object& object = Parent().AddObject("ball", camera->Position());
				object.GetComponent<RigidBodyComponent>()->ApplyImpulse(
						quat(camera->Rotation()) * vec4(0, 0, -1000000, 0));
			}
		}
	}

	void OnMouseMove(const vec2& position) override {
		if (InputSource()->HasFocus()) {
			return;
		}

		auto bounds = InputSource()->GetBounds();
		auto camera = Parent().GetCamera("main_camera");
		vec3 dir = camera->RayDirection(bounds.width, bounds.height, position);

		auto rigidBody = Parent().GetScript<PhysicsScene>()->ShootRay(camera->Position(), dir, 0.01f, 100.0f);

		if (!rigidBody) {
			if (_selected_object) {
				_selected_object->GetComponent<ModelRenderComponent>()->SetMaterial(_original_material);
				_selected_object = nullptr;
			}
		} else {
			auto obj = rigidBody->Parent();

			if (_selected_object != obj) {
				if (_selected_object) {
					_selected_object->GetComponent<ModelRenderComponent>()->SetMaterial(_original_material);
				}

				_selected_object = obj;
				_original_material = _selected_object->GetComponent<ModelRenderComponent>()->GetMaterial();

				Material m = Material({ 0.8f, 0.1f, 0.1f, 1.0f }, 0.7f, 0.0f);
				_selected_object->GetComponent<ModelRenderComponent>()->SetMaterial(m);
			}
		}
	}

private:
	Object *_selected_object = nullptr;
	Material _original_material;

};

class CrosshairElement : public Element {

public:
	void Draw(float dt) const override {
		const Bounds& bounds = GetBounds();

		_DrawColoredQuad(
				Bounds { unsigned(bounds.x + bounds.width * 0.45f), bounds.y, unsigned(bounds.width * 0.1f), bounds.height },
				{ 1.0f, 1.0f, 1.0f, 0.2f });
		_DrawColoredQuad(
				Bounds { bounds.x, unsigned(bounds.y + bounds.height * 0.45f), bounds.width, unsigned(bounds.height * 0.1f) },
				{ 1.0f, 1.0f, 1.0f, 0.2f });
	}

	bool IsOpaque() override {
		return false;
	}

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 1.0f, 1.0f, 1.0f }));
	rigidBodyComponent.SetMass(10000.0f);
	rigidBodyComponent.SetBounciness(0.05f);

	return blueprint;
}

static Blueprint createFloorBlueprint() {
	Blueprint blueprint("floor");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetScale({ 200.0f, 1.0f, 200.0f });
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 200.0f, 1.0f, 200.0f }));

	return blueprint;
}

static Blueprint createBallBlueprint() {
	Blueprint blueprint("ball");

	ModelResource& model = ResourceManager::GetModel("golf_ball.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 1.0f, 1.0f, 1.0f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetScale({ 1.5f, 1.5f, 1.5f });
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Sphere(1.5f));
	rigidBodyComponent.SetMass(18000.0f);
	rigidBodyComponent.SetBounciness(0.3f);

	return blueprint;
}

static Blueprint createRampBlueprint() {
	Blueprint blueprint("ramp");

	ModelResource& model = ResourceManager::GetModel("cube.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.0f));
	modelRenderComponent.SetScale({ 8.0f, 1.0f, 10.0f });
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 8.0f, 1.0f, 10.0f }));

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddScript<GameInputScript>();

	auto& physicsScene = description.AddScript<PhysicsScene>();
	physicsScene.SetGravity({ 0.0f, -9.81f, 0.0f });

	auto& eulerCameraController = description.AddScript<EulerCameraController>();
	eulerCameraController.SetCamera("main_camera");
	eulerCameraController.SetActive(false);

	for (int i = -2; i <= 2; i++) {
		for (int j = 0; j < 5; j++) {
			description.AddObject("cube", { 2.1f * i, 2.1f * j, 2.1f * i });
		}
	}

	description.AddObject("floor", { 0, -2, 0 });
	description.AddObject("ramp", { -16, 5, 0 }, quat(vec3(0, 0, -0.6f)));
	description.AddObject("ramp", { 16, 4, 0 }, quat(vec3(0, 0, 0.8f)));

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { -25, 15, 0 }, { -0.5f, -M_PI / 2.0f, 0.0f });

	return description;
}

class SandboxGame : public Game {
	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createCubeBlueprint());
		Engine::RegisterBlueprint(createFloorBlueprint());
		Engine::RegisterBlueprint(createBallBlueprint());
		Engine::RegisterBlueprint(createRampBlueprint());
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
		config.vsync = true;

		Engine::SetDisplayConfiguration(std::move(config));

		ResourceManager::SetResourcePath("Resources");
	}

	void Start() override {
		Engine::SetActiveScene("main");

		Container ui;

		SceneElement *sceneElement = ui.InsertElement(SceneElement("main_camera"));
		ui.AddConstraint(sceneElement, Constraint::TOP_LEFT, nullptr, Constraint::TOP_LEFT);
		ui.AddConstraint(sceneElement, Constraint::BOTTOM_RIGHT, nullptr, Constraint::BOTTOM_RIGHT);
		sceneElement->SetFocusable(false);

		const auto& resolution = Engine::GetDisplayConfiguration().resolution;
		int size = std::max(resolution.width, resolution.height) / 80;
		int horizontalMargin = (resolution.width - size) / 2;
		int verticalMargin = (resolution.height - size) / 2;

		CrosshairElement *crosshairElement = ui.InsertElement(CrosshairElement());
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT, horizontalMargin);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT, horizontalMargin);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP, verticalMargin);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM, verticalMargin);

		Engine::GetUI().SetContainer(std::move(ui));
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
