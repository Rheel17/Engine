/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "PlayerController.h"

#include "Wowie3.h"
#include "TutorialOverlay.h"
#include "GameOverOverlay.h"

PlayerController::PlayerController(rheel::Entity& camera) :
		_camera(camera) {}

void PlayerController::Update() {
	constexpr static float velocity = 3.0f;
	constexpr static float grace_period = 0.2f;
	float step_size = dt * velocity;
	const auto& maze = static_cast<Wowie3&>(GetEntity().GetScene().GetGame()).GetCurrentMaze(); // NOLINT (safe)

	_check_key_presses();

	// check for game-over states

	// out of maze
	if (!_ended && _source_location.x == maze.GetExit().x && _source_location.y == maze.GetExit().y) {
		_ended = true;
		GetEntity().GetScene().GetRootComponent<game_over_show>()->show = 0.5f;
	}

	if (_ended) {
		vec3 move{
				static_cast<float>(_direction_x[(int) _direction]) * step_size,
				0.0f,
				static_cast<float>(_direction_y[(int) _direction]) * step_size
		};

		GetEntity().transform.Move(move);
		_camera.transform.SetTranslation(GetEntity().transform.GetTranslation() + Wowie3::camera_offset);
		return;
	}

	if (!_moving) {
		_source_location = { static_cast<int>(maze.GetGridWidth() / 2), static_cast<int>(maze.GetGridHeight() / 2) };
		_target_location = { static_cast<int>(maze.GetGridWidth() / 2), static_cast<int>(maze.GetGridHeight() / 2) };
		return;
	}

	// first movement: initial source/target
	if (_source_location.x == _target_location.x && _source_location.y == _target_location.y) {
		_target_location.x += (int) _direction_x[(int) _direction];
		_target_location.y += (int) _direction_y[(int) _direction];
		GetEntity().GetScene().GetRootComponent<tutorial_hide>()->hide = true;
	}

	// current movement, up to the finished fraction or maximum velocity.
	// if we do not change direction, we can even complete the whole movement in
	// one go.
	bool direction_change = _direction != _queued_direction;
	float finish_fraction = direction_change ? std::min(1.0f - _current_fraction, step_size) : step_size;
	float remainder_fraction = step_size - finish_fraction;

	vec3 move{
			static_cast<float>(_direction_x[(int) _direction]) * finish_fraction,
			0.0f,
			static_cast<float>(_direction_y[(int) _direction]) * finish_fraction
	};

	GetEntity().transform.Move(move);
	_camera.transform.SetTranslation(GetEntity().transform.GetTranslation() + Wowie3::camera_offset);
	_current_fraction += finish_fraction;

	// in a certain interval: mark the target tile as visited
	static_assert(grace_period + 0.15f <= 0.4f);
	if (grace_period + 0.15f <= _current_fraction && _current_fraction <= 0.6f) {
		static_cast<Wowie3&>(GetEntity().GetScene().GetGame()).MarkVisited(_target_location); // NOLINT (safe)
	}

	// move to the new tile if needed
	if (!direction_change) {
		if (_current_fraction >= 1.0f) {
			_current_fraction--;
			_source_location = _target_location;
			_target_location = _new_target(_source_location, _direction);
		}
		return;
	}

	// At this point, we have a direction change. There are two moments when a
	// direction change will actually be performed:
	//   1) We just finished the previous tile->tile movement exactly
	//   2) A short grace period in a new tile->tile movement
	//
	// In all other cases, the system will wait until the current tile->tile
	// movement has finished.

	// Normal direction change
	if (remainder_fraction > 0.0f) {
		// snap everything to the grid
		GetEntity().transform.SetTranslation(_snap_xz(GetEntity().transform.GetTranslation()));
		_camera.transform.SetTranslation(GetEntity().transform.GetTranslation() + Wowie3::camera_offset);
		_current_fraction = 0.0f;

		// calculate new souce/target
		location new_source = _target_location;
		location new_target = _new_target(new_source, _queued_direction);

		// check whether the new target is valid
		if (!_is_valid_tile(new_target, maze)) {
			_queued_direction = _direction;
			goto finish; // NOLINT (goto handy af)
		}

		// check whether the direction change was a turnaround, and whether that
		// turnaround is valid.
		if (_queued_direction == _opposites[(int) _direction]) {
			// there is a turnaround

			if (_is_valid_tile(_new_target(_target_location, _direction), maze)) {
				// the straight-ahead tile is valid, so the turnaround is not
				// valid
				_queued_direction = _direction;
				goto finish; // NOLINT (goto handy af)
			}
		}

		finish:
		// perform the actual direction change
		_direction = _queued_direction;
		_source_location = _target_location;
		_target_location = _new_target(_source_location, _direction);

		// finish the movement
		move = {
				static_cast<float>(_direction_x[(int) _direction]) * remainder_fraction,
				0.0f,
				static_cast<float>(_direction_y[(int) _direction]) * remainder_fraction
		};

		GetEntity().transform.Move(move);
		_camera.transform.SetTranslation(GetEntity().transform.GetTranslation() + Wowie3::camera_offset);
		_current_fraction += remainder_fraction;
	} else if (_current_fraction < grace_period) { // Grace period direction change
		// calculate new souce/target
		location new_source = _source_location;
		location new_target = _new_target(new_source, _queued_direction);

		// check whether the new target is valid
		if (!_is_valid_tile(new_target, maze)) {
			_queued_direction = _direction;
			return;
		}

		// check whether the direction change was a turnaround, and whether that
		// turnaround is valid.
		if (_queued_direction == _opposites[(int) _direction]) {
			// there is a turnaround

			if (_is_valid_tile(_target_location, maze)) {
				// the straight-ahead tile is valid, so the turnaround is not
				// valid
				_queued_direction = _direction;
				return;
			}
		}

		// perform the actual direction change
		_direction = _queued_direction;
		_target_location = _new_target(_source_location, _direction);

		// snap everything to the grid
		GetEntity().transform.SetTranslation(_snap_xz(GetEntity().transform.GetTranslation()));
		_camera.transform.SetTranslation(GetEntity().transform.GetTranslation() + Wowie3::camera_offset);
		_current_fraction = 0.0f;
	}
}

void PlayerController::_check_key_presses() {
	auto set = [this](Direction dir) {
		_queued_direction = dir;

		if (!_moving) {
			_direction = dir;
		}

		_moving = true;
	};

	if (IsKeyPressed(rheel::Input::Key::KEY_UP) ||
			IsKeyPressed(rheel::Input::Key::KEY_W) ||
			IsKeyPressed(rheel::Input::Key::KEY_I)) {
		set(Direction::UP);
	} else if (IsKeyPressed(rheel::Input::Key::KEY_LEFT) ||
			IsKeyPressed(rheel::Input::Key::KEY_A) ||
			IsKeyPressed(rheel::Input::Key::KEY_J)) {
		set(Direction::LEFT);
	} else if (IsKeyPressed(rheel::Input::Key::KEY_DOWN) ||
			IsKeyPressed(rheel::Input::Key::KEY_S) ||
			IsKeyPressed(rheel::Input::Key::KEY_K)) {
		set(Direction::DOWN);
	} else if (IsKeyPressed(rheel::Input::Key::KEY_RIGHT) ||
			IsKeyPressed(rheel::Input::Key::KEY_D) ||
			IsKeyPressed(rheel::Input::Key::KEY_L)) {
		set(Direction::RIGHT);
	}
}

bool PlayerController::_is_valid_tile(location target, const Maze& maze) {
	if (target.x < 0 || target.x >= maze.GetGridWidth()) {
		return true;
	}

	if (target.y < 0 || target.y >= maze.GetGridHeight()) {
		return true;
	}

	return !maze.IsWall(target);
}

location PlayerController::_new_target(location source, Direction direction) {
	return { source.x + _direction_x[(int) direction], source.y + _direction_y[(int) direction] };
}

vec3 PlayerController::_snap_xz(const vec3& v) {
	return vec3{ std::round(v.x), v.y, std::round(v.z) };
}
