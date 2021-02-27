/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_WOWIE3_H
#define ENGINE_WOWIE3_H

#include <RheelEngine.h>

#include "Maze.h"

class Wowie3 : public rheel::Game {

public:
	static constexpr vec3 camera_offset{ 0.0f, 2.9f, 0.6f };

	Wowie3() :
			rheel::Game(_get_display_configuration()) {}

	void Start() override;
	void NewGame();

	const Maze& GetCurrentMaze() const;

private:
	rheel::ScenePointer _create_maze_scene(const Maze& maze);
	static rheel::DisplayConfiguration _get_display_configuration();

	std::optional<Maze> _current_maze;

};

#endif
