/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "Maze.h"

#include <stack>

llvm::SmallVector<location, 4> location::neighbors(const Maze& maze) const {
	llvm::SmallVector<location, 4> locations{};

	auto test = [this, &locations, &maze](int dx, int dy) {
		int new_x = x + dx;
		int new_y = y + dy;
		location new_loc{ new_x, new_y };

		if (0 <= new_x && new_x < maze.GetGridWidth() &&
				0 <= new_y && new_y < maze.GetGridHeight()) {
			locations.push_back(new_loc);
		}
	};

	test(-2, 0);
	test(2, 0);
	test(0, -2);
	test(0, 2);

	return locations;
}

unsigned int Maze::GetGridWidth() const {
	return _width;
}

unsigned int Maze::GetGridHeight() const {
	return _height;
}

bool Maze::IsWall(int x, int y) const {
	return (*this)[{ x, y }] == wall;
}

bool Maze::IsWall(location loc) const {
	return (*this)[loc] == wall;
}

bool Maze::IsValid() const {
	Maze copy(*this);

	// mark all floor tiles as unvisited
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			if (copy[{ x, y }] == floor) {
				copy[{ x, y }] = unvisited;
			}
		}
	}

	// iterative depth-first search
	std::stack<location> fringe;
	fringe.push({ 1, 1 });

	while (!fringe.empty()) {
		location loc = fringe.top();
		fringe.pop();

		if (copy[loc] == unvisited) {
			copy[loc] = floor;

			auto ns = loc.neighbors(copy);

			for (auto l : ns) {
				location mid{ (l.x + loc.x) / 2, (l.y + loc.y) / 2 };
				if (copy[mid] != Maze::wall) {
					fringe.push(l);
				}
			}
		}
	}

	// check if all cells were visited
	for (int x = 1; x < _width; x += 2) {
		for (int y = 1; y < _height; y += 2) {
			if (copy[{ x, y }] == Maze::unvisited) {
				return false;
			}
		}
	}

	return true;
}

location Maze::GetExit() const {
	return _exit;
}

location Maze::GetExitDelta() const {
	return _delta_exit;
}

Maze::Maze(unsigned int grid_width, unsigned int grid_height) :
		_width(grid_width),
		_height(grid_height),
		_cells(grid_width * grid_height) {}

char& Maze::operator[](location loc) {
	return _cells[loc.x * _height + loc.y];
}

char Maze::operator[](location loc) const {
	return _cells[loc.x * _height + loc.y];
}
