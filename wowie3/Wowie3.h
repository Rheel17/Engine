/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_WOWIE3_H
#define ENGINE_WOWIE3_H

#include <RheelEngine.h>

#include "Maze.h"

class Wowie3 : public rheel::Game {
	struct score_updater : public rheel::Component {
		static constexpr rheel::ComponentId id = 1;
		rheel::TextElement* text_element;
		int score = 0;

		void Update() override;
	};

public:
	static constexpr vec3 camera_offset{ 0.0f, 2.9f, 0.6f };

	Wowie3() :
			rheel::Game(_get_display_configuration()) {}

	void Start() override;
	void NewGame();

	const Maze& GetCurrentMaze() const;

	void MarkVisited(location loc);

private:
	rheel::ScenePointer _create_maze_scene(const Maze& maze);
	static rheel::DisplayConfiguration _get_display_configuration();

	std::optional<Maze> _current_maze;
	std::optional<Maze> _visited_status;
	int _num_tiles = 0;
	int _cur_tile_count = 0;

};

#endif
