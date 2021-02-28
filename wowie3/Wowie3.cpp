/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "Wowie3.h"

#include "MazeGenerator.h"
#include "PlayerController.h"
#include "MainMenu.h"
#include "GameOverOverlay.h"

void Wowie3::score_updater::Update() {
	if (text_element) {
		text_element->text = std::to_string(score) + "%";
	}
}

void Wowie3::Start() {
	// main menu scene
	auto scene = CreateScene();

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

	// walls
	static auto wall_model = rheel::StaticModelGeneratorBox({ 1.0f, 2.5f, 1.0f })();

	for (int x = -10; x <= 10; x++) {
		for (int y = -10; y <= 10; y++) {
			if (-2 <= x && x <= 3 && -2 <= y && y <= 2) {
				continue;
			}

			auto& cube = scene->AddEntity(rheel::Transform(vec3{ static_cast<float>(x) - 0.5f, -0.475f, static_cast<float>(y) - 0.5f }));
			cube.AddComponent<rheel::ModelRenderComponent>(wall_model, rheel::Material(0xCB5B5A, 0.7f, 0.1f));
		}
	}

	SetActiveScene(std::move(scene));

	rheel::Container ui;

	auto* scene_element = ui.InsertElement(rheel::SceneElement("main_camera"_id));
	ui.AddConstraint(scene_element, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(scene_element, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	auto* vignette_element = ui.InsertElement(rheel::VignetteElement(rheel::Color{ 0.0f, 0.0f, 0.0f, 0.65f }, 0.5f, 2.0f));
	ui.AddConstraint(vignette_element, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(vignette_element, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	auto* main_menu = ui.InsertElement(MainMenu(*this));
	ui.AddConstraint(main_menu, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(main_menu, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	GetUI().SetContainer(std::move(ui));
	main_menu->RequestFocus();
}

void Wowie3::NewGame() {
	static auto maze_generator = MazeGenerator(17, 11);
	// static auto maze_generator = MazeGenerator(9, 7);
	_current_maze = maze_generator.Generate();
	_visited_status = _current_maze;

	// calculate number of tiles needed for 100%
	_num_tiles = 0;
	_cur_tile_count = 0;

	for (int i = 0; i < _visited_status->GetGridWidth(); i++) {
		for (int j = 0; j < _visited_status->GetGridHeight(); j++) {
			if (!_visited_status->IsWall(i, j)) {
				_num_tiles++;
				_visited_status->operator[]({ i, j }) = Maze::unvisited;
			}
		}
	}

	// remove 5x5 center
	_num_tiles -= 25;

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			_visited_status->operator[](
					{ static_cast<int>(_visited_status->GetGridWidth() / 2 + i), static_cast<int>(_visited_status->GetGridHeight() / 2 + j) }) = Maze::floor;
		}
	}

	// remove exit corridor
	location loc = _visited_status->GetExit();
	location d = _visited_status->GetExitDelta();

	_visited_status->operator[](loc) = Maze::floor;
	loc.x += d.x;
	loc.y += d.y;

	auto ns = loc.neighbors(*_visited_status);

	int remove_corridor = 2;

	do {
		if (_visited_status->IsWall(loc.x + d.y, loc.y - d.x) && _visited_status->IsWall(loc.x - d.y, loc.y + d.x)) {
			_visited_status->operator[](loc) = Maze::floor;
			remove_corridor++;
			loc.x += d.x;
			loc.y += d.y;
			continue;
		}

		break;
	} while (true);

	_num_tiles -= remove_corridor;

	SetActiveScene(_create_maze_scene(*_current_maze));

	rheel::Container ui;

	auto* scene_element = ui.InsertElement(rheel::SceneElement("main_camera"_id));
	ui.AddConstraint(scene_element, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(scene_element, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	auto* vignette_element = ui.InsertElement(rheel::VignetteElement(rheel::Color{ 0.0f, 0.0f, 0.0f, 0.65f }, 0.5f, 2.0f));
	ui.AddConstraint(vignette_element, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(vignette_element, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	auto* score_element = ui.InsertElement(rheel::TextElement("0%", 0.33f, 0xECDCB0, font));
	ui.AddWidthRelativeConstraint(score_element, rheel::Constraint::TOP, nullptr, rheel::Constraint::TOP, 0.02f);
	ui.AddWidthRelativeConstraint(score_element, rheel::Constraint::LEFT, nullptr, rheel::Constraint::LEFT, 0.03f);
	ui.AddWidthRelativeConstraint(score_element, rheel::Constraint::BOTTOM, nullptr, rheel::Constraint::TOP, -0.05f);
	ui.AddWidthRelativeConstraint(score_element, rheel::Constraint::RIGHT, nullptr, rheel::Constraint::LEFT, -0.10f);

	auto* tutorial_overlay = ui.InsertElement(TutorialOverlay(*this));
	ui.AddConstraint(tutorial_overlay, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(tutorial_overlay, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	auto* game_over_overlay = ui.InsertElement(GameOverOverlay(*this));
	ui.AddConstraint(game_over_overlay, rheel::Constraint::TOP_LEFT, nullptr, rheel::Constraint::TOP_LEFT);
	ui.AddConstraint(game_over_overlay, rheel::Constraint::BOTTOM_RIGHT, nullptr, rheel::Constraint::BOTTOM_RIGHT);

	GetUI().SetContainer(std::move(ui));
	scene_element->RequestFocus();

	GetActiveScene()->GetRootComponent<score_updater>()->text_element = score_element;
	GetActiveScene()->GetRootComponent<tutorial_hide>()->element = tutorial_overlay;
	GetActiveScene()->GetRootComponent<game_over_show>()->element = game_over_overlay;
	GetActiveScene()->GetRootComponent<game_over_show>()->text_element = score_element;
}

const Maze& Wowie3::GetCurrentMaze() const {
	return *_current_maze;
}

void Wowie3::MarkVisited(location loc) {
	auto& val = _visited_status->operator[](loc);
	if (val == Maze::unvisited) {
		val = Maze::floor;
		_cur_tile_count++;
		GetActiveScene()->GetRootComponent<score_updater>()->score = (100 * _cur_tile_count) / _num_tiles;
	}
}

bool Wowie3::HasVisitedAll() const {
	return _cur_tile_count >= _num_tiles;
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

	// ui related
	scene->AddRootComponent<score_updater>();
	scene->AddRootComponent<tutorial_hide>();
	scene->AddRootComponent<game_over_show>();

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

const rheel::Font& Wowie3::_get_font() {
	rheel::Font::RegisterFont("Montserrat-Regular.ttf", "font");
	return rheel::Font::GetFont("font");
}

RHEEL_ENGINE_ENTRY(Wowie3)
