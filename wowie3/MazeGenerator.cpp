/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "MazeGenerator.h"

#include <stack>

MazeGenerator::MazeGenerator(unsigned int width, unsigned int height) :
		_width(width),
		_height(height),
		// _rng(std::random_device{}() ^ reinterpret_cast<std::uintptr_t>(this)) {} // NOLINT (safety not needed)
		_rng(0) {}

Maze MazeGenerator::Generate() {
	while (true) {
		Maze maze = _do_generate();
		if (maze.IsValid()) {
			return maze;
		}
	}
}

Maze MazeGenerator::_do_generate() {
	// initialize the maze
	Maze maze(_width * 2 + 1, _height * 2 + 1);

	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			maze[{ x * 2 + 1, y * 2 + 1 }] = Maze::_unvisited;
		}
	}

	// create the exit
	auto side = std::uniform_int_distribution<int>(0, 3)(_rng);
	location exit{};

	int exit_corridor_dx = 0;
	int exit_corridor_dy = 0;

	switch (side) { // NOLINT (all paths covered)
		case 0:
			exit = { std::uniform_int_distribution<int>(0, _width - 1)(_rng) * 2 + 1, 1 };
			maze[location{ exit.x, 0 }] = Maze::_floor;
			exit_corridor_dy = 1;
			break;
		case 1:
			exit = { std::uniform_int_distribution<int>(0, _width - 1)(_rng) * 2 + 1, static_cast<int>(_height * 2 - 1) };
			maze[location{ exit.x, static_cast<int>(_height * 2) }] = Maze::_floor;
			exit_corridor_dy = -1;
			break;
		case 2:
			exit = { 0, std::uniform_int_distribution<int>(0, _height - 1)(_rng) * 2 + 1 };
			maze[location{ 0, exit.y }] = Maze::_floor;
			exit_corridor_dx = 1;
			break;
		case 3:
			exit = { static_cast<int>(_width * 2 - 1), std::uniform_int_distribution<int>(0, _height - 1)(_rng) * 2 + 1 };
			maze[location{ static_cast<int>(_width * 2), exit.y }] = Maze::_floor;
			exit_corridor_dx = -1;
			break;
	}

	// use randomized iterative depth-first search, starting at the neighbor of
	// the exit
	std::stack<location> fringe;
	fringe.push(exit.neighbors(maze)[0]);

	while (!fringe.empty()) {
		// Pop a cell and make it a current cell
		auto loc = fringe.top();
		fringe.pop();

		maze[loc] = Maze::_floor;

		// If the current cell has any neighbors which have not been visited
		auto ns = loc.neighbors(maze);
		auto iter = std::remove_if(ns.begin(), ns.end(), [&maze](location l) { return maze[l] != Maze::_unvisited; }); // NOLINT

		if (iter != ns.end()) {
			ns.erase(iter, ns.end());
		}

		if (!ns.empty()) {
			// push the current cell to the stack
			fringe.push(loc);

			// choose one of the unvisited neighbors
			auto n = ns[std::uniform_int_distribution<std::size_t>(0, ns.size() - 1)(_rng)];

			// remove the wall between the current cell and the chosen cell
			maze[{ (loc.x + n.x) / 2, (loc.y + n.y) / 2 }] = Maze::_floor;

			// mark the chosen cell as visited and push it to the stack
			maze[n] = Maze::_floor;
			fringe.push(n);
		}
	}

	// empty the center 3 x 3
	for (int x = static_cast<int>(_width) - 2; x <= _width + 2; x++) {
		for (int y = static_cast<int>(_height) - 2; y <= _height + 2; y++) {
			maze[{ x, y }] = Maze::_floor;
		}
	}

	// create exit corridor
	location loc = exit;

	for (int i = 0; i < 5; i++) {
		maze[loc] = Maze::_floor;
		maze[{ loc.x + exit_corridor_dy, loc.y - exit_corridor_dx }] = Maze::_wall;
		maze[{ loc.x - exit_corridor_dy, loc.y + exit_corridor_dx }] = Maze::_wall;

		loc.x += exit_corridor_dx;
		loc.y += exit_corridor_dy;
	}

	return maze;
}
