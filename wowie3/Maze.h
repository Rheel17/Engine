/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MAZE_H
#define ENGINE_MAZE_H

#include <vector>

#include <llvm/ADT/SmallVector.h>

class Maze;

struct location {
	int x, y;

	llvm::SmallVector<location, 4> neighbors(const Maze& maze) const;
};

class Maze {
	friend class MazeGenerator;

	static constexpr char _unvisited = -1;
	static constexpr char _wall = 0;
	static constexpr char _floor = 1;

public:
	unsigned int GetGridWidth() const;
	unsigned int GetGridHeight() const;

	bool IsWall(int x, int y) const;
	bool IsWall(location loc) const;

	bool IsValid() const;

private:
	Maze(unsigned int grid_width, unsigned int grid_height);

	char& operator[](location loc);
	char operator[](location loc) const;

	unsigned int _width;
	unsigned int _height;
	std::vector<char> _cells;

};

#endif
