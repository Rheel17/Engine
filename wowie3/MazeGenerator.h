/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MAZEGENERATOR_H
#define ENGINE_MAZEGENERATOR_H

#include "Maze.h"

#include <RheelEngine.h>

#include <random>

class MazeGenerator {

public:
	MazeGenerator(unsigned int width, unsigned int height);

	Maze Generate();

private:
	unsigned int _width;
	unsigned int _height;
	std::mt19937 _rng;

	Maze _do_generate();

};

#endif
