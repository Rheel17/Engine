#include "GlyphDistanceField.h"

#include <iostream>
#include <limits>

#include <string_format.h>

namespace rheel {

GlyphDistanceField::GlyphDistanceField(unsigned char *data, unsigned width, unsigned height) :
		_width(width), _height(height) {

	float inf = 123456.789f;

	// create the grid array
	_grid1 = new vec2 *[_height + 2];
	_grid2 = new vec2 *[_height + 2];

	for (unsigned y = 0; y < _height + 2; y++) {
		_grid1[y] = new vec2[_width + 2];
		_grid2[y] = new vec2[_width + 2];
	}

	// fill the bitmap
	for (unsigned y = 0; y < _height; y++) {
		for (unsigned x = 0; x < _width; x++) {
			if (data[x + y * _width] > 0) {
				_grid1[y + 1][x + 1] = { 0, 0 };
				_grid2[y + 1][x + 1] = { inf, inf };
			} else {
				_grid1[y + 1][x + 1] = { inf, inf };
				_grid2[y + 1][x + 1] = { 0, 0 };
			}
		}
	}

	// fill the border
	for (unsigned x = 0; x < _width + 2; x++) {
		_grid1[0][x] = { inf, inf };
		_grid2[0][x] = { 0, 0 };

		_grid1[_height + 1][x] = { inf, inf };
		_grid2[_height + 1][x] = { 0, 0 };
	}

	for (unsigned y = 1; y < _height + 1; y++) {
		_grid1[y][0] = { inf, inf };
		_grid2[y][0] = { 0, 0 };

		_grid1[y][_width + 1] = { inf, inf };
		_grid2[y][_width + 1] = { 0, 0 };
	}

	// create the output data array
	_data = new float[_width * _height];

	// compute and create the SDF
	_Compute(_grid1);
	_Compute(_grid2);
	_Convert();

	// delete the grids
	for (unsigned y = 0; y < _height + 2; y++) {
		delete[] _grid1[y];
		delete[] _grid2[y];
	}

	delete[] _grid1;
	delete[] _grid2;
}

GlyphDistanceField::~GlyphDistanceField() {
	delete[] _data;
}

float *GlyphDistanceField::Data() const {
	return _data;
}

void GlyphDistanceField::_Handle(_Grid grid, vec2& p, unsigned x, unsigned y, int dx, int dy) {
	if ((x == 0 && dx < 0) || (y == 0 && dy < 0)) {
		return;
	}

	if ((x >= _width + 1 && dx > 0) || (y >= _height + 1 && dy > 0)) {
		return;
	}

	vec2 other = grid[y + dy][x + dx];
	other += vec2 { dx, dy };

	if (glm::dot(other, other) < glm::dot(p, p)) {
		p = other;
	}
}

void GlyphDistanceField::_Compute(_Grid grid) {
	for (unsigned y = 0; y < _height + 2; y++) {
		for (unsigned x = 0; x < _width + 2; x++) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, -1, 0);
			_Handle(grid, p, x, y, 0, -1);
			_Handle(grid, p, x, y, -1, -1);
			_Handle(grid, p, x, y, 1, -1);
			grid[y][x] = p;
		}

		for (int x = _width + 1; x >= 0; x--) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, 1, 0);
			grid[y][x] = p;
		}
	}

	for (int y = _height + 1; y >= 0; y--) {
		for (int x = _width + 1; x >= 0; x--) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, 1, 0);
			_Handle(grid, p, x, y, 0, 1);
			_Handle(grid, p, x, y, -1, 1);
			_Handle(grid, p, x, y, 1, 1);
			grid[y][x] = p;
		}

		for (unsigned x = 0; x < _width + 2; x++) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, -1, 0);
			grid[y][x] = p;
		}
	}
}

void GlyphDistanceField::_Convert() {
	for (unsigned y = 0; y < _height; y++) {
		for (unsigned x = 0; x < _width; x++) {
			_data[y + _width * x] = glm::length(_grid1[y + 1][x + 1]) - glm::length(_grid2[y + 1][x + 1]);
		}
	}
}

}
