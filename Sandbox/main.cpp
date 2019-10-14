/*
 * Copyright © 2019 Levi van Rheenen. All rights reserved.
 */
#include <RheelEngine.h>

using namespace rheel;

class GameInputScript : public Script {
	SCRIPT_INIT(GameInputScript);

public:
	void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) override {
		if (button == Input::MouseButton::LEFT && InputSource()->HasFocus()) {
			if (InputSource()->HasFocus()) {
				auto camera = Parent().GetCamera("main_camera");
				Object& object = Parent().AddObject("ball", camera->Position());
				object.GetComponent<RigidBodyComponent>()->ApplyImpulse(
						quat(camera->Rotation()) * vec4(0, 0, -100, 0));
			}
		}
	}

};

class RemoveComponent : public ComponentOld {
	COMPONENT_INIT(RemoveComponent);

public:
	void OnUpdate(float dt) override {
		if (Parent()->Position().y < -3) {
			Parent()->ParentScene()->RemoveObject(*Parent());
		}
	}

};

static Blueprint createCubeBlueprint() {
	Blueprint blueprint("cube");

	ModelResource& model = ModelResource::Box({ 0.5f, 0.5f, 0.5f });

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.9f, 0.6f, 0.2f, 1.0f }, 0.7f, 0.5f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 0.5f, 0.5f, 0.5f }));
	rigidBodyComponent.SetMass(5.0f);
	rigidBodyComponent.SetBounciness(0.05f);

	blueprint.AddComponent<RemoveComponent>();

	return blueprint;
}

static Blueprint createFloorBlueprint() {
	Blueprint blueprint("floor");

	ModelResource& model = ModelResource::Box({ 15.0f, 0.5f, 15.0f });

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.6f, 0.7f, 1.0f, 1.0f }, 0.7f, 0.2f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 15.0f, 0.5f, 15.0f }));

	return blueprint;
}

static Blueprint createBallBlueprint() {
	Blueprint blueprint("ball");

	ModelResource& model = ResourceManager::GetModel("golf_ball.dae");

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 1.0f, 1.0f, 1.0f, 1.0f }, 0.7f, 0.05f));
	modelRenderComponent.SetScale(0.25f);
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Sphere(0.25f));
	rigidBodyComponent.SetMass(3.0f);
	rigidBodyComponent.SetBounciness(0.3f);

	blueprint.AddComponent<RemoveComponent>();

	return blueprint;
}

static Blueprint createRampBlueprint() {
	Blueprint blueprint("ramp");

	ModelResource& model = ModelResource::Box({ 4.0f, 0.1f, 5.0f });

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 0.3f, 0.7f, 0.4f, 1.0f }, 0.7f, 0.2f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Box({ 4.0f, 0.1f, 5.0f }));

	return blueprint;
}

static Blueprint createPlayerBlueprint() {
	Blueprint blueprint("player");

	ModelResource& model = ModelResource::Capsule(0.4f, 1.0f);

	auto& modelRenderComponent = blueprint.AddComponent<ModelRenderComponent>();
	modelRenderComponent.SetMaterial(Material({ 1.0f, 0.9f, 0.8f, 1.0f }, 0.7f, 0.4f));
	modelRenderComponent.SetModel(model);

	auto& rigidBodyComponent = blueprint.AddComponent<RigidBodyComponent>();
	rigidBodyComponent.SetShape(PhysicsShape::Capsule(0.4f, 1.0f));
	rigidBodyComponent.SetMass(80.0f);
	rigidBodyComponent.SetBounciness(0.0f);

	return blueprint;
}

static SceneDescription createSceneDescription() {
	SceneDescription description("main");

	description.AddScript<GameInputScript>();

	auto& physicsScene = description.AddScript<PhysicsScene>();
	physicsScene.SetGravity({ 0.0f, -9.81f, 0.0f });

	auto& eulerCameraController = description.AddScript<EulerCameraController>();
	eulerCameraController.SetCamera("main_camera");

	auto& listenerFollow = description.AddScript<ListenerFollow>();
	listenerFollow.SetCamera("main_camera");

	for (int i = -2; i <= 2; i++) {
		for (int j = 0; j < 5; j++) {
			description.AddObject("cube", { 1.1f * i, 1.1f * j + 0.5f, 1.1f * i });
		}
	}

	description.AddObject("floor", { 0, -0.25, 0 });
	description.AddObject("ramp", { -8, 3, 0 }, quat(vec3(0, 0, -0.6f)));
	description.AddObject("ramp", { 8, 2, 0 }, quat(vec3(0, 0, 0.8f)));
	description.AddObject("player", { 0, 2, 10 });

	description.AddLight("main_light", DirectionalLight({ 1, 1, 1, 1 }, { 0.2f, -2.0f, -1.0f }), 100.0f);
	description.AddCamera("main_camera", 75.0f, 0.01f, 100.0f, { -12, 7.5f, 0 }, { -0.5f, -M_PI / 2.0f, 0.0f });

	return description;
}

class SandboxGame : public Game {
	void RegisterBlueprints() override {
		Engine::RegisterBlueprint(createCubeBlueprint());
		Engine::RegisterBlueprint(createFloorBlueprint());
		Engine::RegisterBlueprint(createBallBlueprint());
		Engine::RegisterBlueprint(createRampBlueprint());
		Engine::RegisterBlueprint(createPlayerBlueprint());
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
		sceneElement->GetPostProcessingStack().SetBloom(Bloom(0.5f, 0.7f, 0.15f, 3.0f, 8));

		CrosshairElement *crosshairElement = ui.InsertElement(CrosshairElement(20));
		ui.AddConstraint(crosshairElement, Constraint::LEFT, nullptr, Constraint::LEFT);
		ui.AddConstraint(crosshairElement, Constraint::RIGHT, nullptr, Constraint::RIGHT);
		ui.AddConstraint(crosshairElement, Constraint::TOP, nullptr, Constraint::TOP);
		ui.AddConstraint(crosshairElement, Constraint::BOTTOM, nullptr, Constraint::BOTTOM);

		Engine::GetUI().SetContainer(std::move(ui));
		sceneElement->RequestFocus();
	}

};

RHEEL_ENGINE_ENTRY(SandboxGame)
