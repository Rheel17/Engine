/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "Wowie3.h"

#include "MazeGenerator.h"
#include "PlayerController.h"

void Wowie3::Start() {
	NewGame();

	rheel::Container ui;

	auto* scene_element = ui.InsertElement(rheel::SceneElement("main_camera"_id));
	ui.AddConstraint(scene_element, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(scene_element, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	GetUI().SetContainer(std::move(ui));
	scene_element->RequestFocus();
}

void Wowie3::NewGame() {
	static auto maze_generator = MazeGenerator(17, 11);
	_current_maze = maze_generator.Generate();

	SetActiveScene(_create_maze_scene(*_current_maze));
}

const Maze& Wowie3::GetCurrentMaze() const {
	return *_current_maze;
}

rheel::ScenePointer Wowie3::_create_maze_scene(const Maze& maze) {
	auto scene = rheel::Game::CreateScene();

	// camera
	auto& camera = scene->AddEntity("main_camera"_id, rheel::Transform(camera_offset, vec3{ -1.4f, 0.0f, 0.0f }));
	camera.AddComponent<rheel::PerspectiveCamera>(75.0f, 0.01f, 100.0f);

	// light
	auto& light = scene->AddEntity("main_light"_id);
	auto& light_component = light.AddComponent<rheel::DirectionalLight>(0xFFFFFF, vec3{ 1.0f, -2.0f, -0.3f });
	light_component.SetShadowDistance(40.0f);

	// background
	static auto background_model = rheel::StaticModelGeneratorBox({ 100.0f, 1.0f, 100.0f })();
	auto& background = scene->AddEntity(rheel::Transform({ 0.0f, -0.84f, 0.0f }));
	background.AddComponent<rheel::ModelRenderComponent>(background_model, rheel::Material(0x2E2E3A, 0.8f, 0.2f));

	// maze
	static auto wall_model = rheel::StaticModelGeneratorBox({ 1.0f, 2.5f, 1.0f })();

	float x_offset = static_cast<float>(maze.GetGridWidth()) / 2.0f;
	float y_offset = static_cast<float>(maze.GetGridHeight()) / 2.0f;

	for (int x = 0; x < maze.GetGridWidth(); x++) {
		for (int y = 0; y < maze.GetGridHeight(); y++) {
			if (!maze.IsWall(x, y)) {
				continue;
			}

			auto& cube = scene->AddEntity(rheel::Transform(vec3{ static_cast<float>(x) - x_offset + 0.5f, -0.475f, static_cast<float>(y) - y_offset + 0.5f }));
			cube.AddComponent<rheel::ModelRenderComponent>(wall_model, rheel::Material(0xCB5B5A, 0.7f, 0.1f));
		}
	}

	// player
	static auto player_model = rheel::StaticModelGeneratorBox({ 0.7f, 0.7f, 0.7f })();

	auto& player = scene->AddEntity();
	player.AddComponent<rheel::ModelRenderComponent>(player_model, rheel::Material(0xECDCB0, 0.8f, 0.01f));
	player.AddComponent<PlayerController>(camera);

	return scene;
}

rheel::DisplayConfiguration Wowie3::_get_display_configuration() {
	rheel::DisplayConfiguration config;
	config.window_mode = rheel::DisplayConfiguration::FULLSCREEN;
	config.shadow_quality = rheel::DisplayConfiguration::SHADOW_HIGH;
	config.aa_mode = rheel::DisplayConfiguration::AntiAliasing::MSAA_4;
	config.vsync = true;
	return config;
}

RHEEL_ENGINE_ENTRY(Wowie3)
