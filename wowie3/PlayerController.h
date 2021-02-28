/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_PLAYERCONTROLLER_H
#define ENGINE_PLAYERCONTROLLER_H

#include <RheelEngine.h>

#include "Maze.h"

class PlayerController : public rheel::InputComponent {
	enum class Direction {
		DOWN, RIGHT, UP, LEFT
	};

public:
	static constexpr rheel::ComponentId id = 0;

	explicit PlayerController(rheel::Entity& camera);

	void Update() override;

private:
	void _check_key_presses();
	bool _is_valid_tile(location target, const Maze& maze);
	location _new_target(location source, Direction direction);

	static constexpr int _direction_x[4]{ 0, 1, 0, -1 };
	static constexpr int _direction_y[4]{ 1, 0, -1, 0 };
	Direction _opposites[4]{ Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT };

	rheel::Entity& _camera;

	bool _ended = false;
	bool _moving = false;
	float _current_fraction = 0.0f;

	Direction _direction{};
	Direction _queued_direction{};

	location _source_location{};
	location _target_location{};

	static vec3 _snap_xz(const vec3& v);

};

#endif
