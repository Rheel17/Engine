/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MAZE_H
#define ENGINE_MAZE_H

#include <vector>

class Maze;

struct location {
	int x, y;

	std::vector<location> neighbors(const Maze& maze) const;
};

class Maze {
	friend class MazeGenerator;

public:
	static constexpr char unvisited = -1;
	static constexpr char wall = 0;
	static constexpr char floor = 1;

	unsigned int GetGridWidth() const;
	unsigned int GetGridHeight() const;

	bool IsWall(int x, int y) const;
	bool IsWall(location loc) const;

	bool IsValid() const;
	location GetExit() const;
	location GetExitDelta() const;

	char& operator[](location loc);
	char operator[](location loc) const;

private:
	Maze(unsigned int grid_width, unsigned int grid_height);

	unsigned int _width;
	unsigned int _height;
	std::vector<char> _cells;
	location _exit{};
	location _delta_exit{};

};

#endif
