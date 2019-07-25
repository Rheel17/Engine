#include "GlyphDistanceField.h"

#include <algorithm>
#include <iostream>
#include <limits>

#include <string_format.h>

namespace rheel {

GlyphDistanceField::GlyphDistanceField(unsigned char *data, unsigned width, unsigned height) :
		_width(width + 2 * PADDING), _height(height + 2 * PADDING) {

	float inf = 123456.789f;

	// create the grid array
	_grid1 = new vec2 *[_height];
	_grid2 = new vec2 *[_height];

	for (unsigned y = 0; y < _height; y++) {
		_grid1[y] = new vec2[_width];
		_grid2[y] = new vec2[_width];
	}

	// fill the bitmap
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (data[x + y * width] > 0) {
				_grid1[y + PADDING][x + PADDING] = { 0, 0 };
				_grid2[y + PADDING][x + PADDING] = { inf, inf };
			} else {
				_grid1[y + PADDING][x + PADDING] = { inf, inf };
				_grid2[y + PADDING][x + PADDING] = { 0, 0 };
			}
		}
	}

	// fill the border
	for (unsigned x = 0; x < _width; x++) {
		for (unsigned y = 0; y < PADDING; y++) {
			_grid1[y][x] = { inf, inf };
			_grid2[y][x] = { 0, 0 };

			_grid1[_height - y - 1][x] = { inf, inf };
			_grid2[_height - y - 1][x] = { 0, 0 };
		}
	}

	for (unsigned y = PADDING; y < _height - PADDING; y++) {
		for (unsigned x = 0; x < PADDING; x++) {
			_grid1[y][x] = { inf, inf };
			_grid2[y][x] = { 0, 0 };

			_grid1[y][_width - x - 1] = { inf, inf };
			_grid2[y][_width - x - 1] = { 0, 0 };
		}
	}

	// create the output data array
	_data.resize(_width * _height);

	// compute and create the SDF
	_Compute(_grid1);
	_Compute(_grid2);
	_Convert();

	// delete the grids
	for (unsigned y = 0; y < _height; y++) {
		delete[] _grid1[y];
		delete[] _grid2[y];
	}

	delete[] _grid1;
	delete[] _grid2;
}

const float *GlyphDistanceField::Data() const {
	return reinterpret_cast<const float *>(_data.data());
}

void GlyphDistanceField::_Handle(_Grid grid, vec2& p, unsigned x, unsigned y, int dx, int dy) {
	if ((x == 0 && dx < 0) || (y == 0 && dy < 0)) {
		return;
	}

	if ((x >= _width - 1 && dx > 0) || (y >= _height - 1 && dy > 0)) {
		return;
	}

	vec2 other = grid[y + dy][x + dx];
	other += vec2 { dx, dy };

	if (glm::dot(other, other) < glm::dot(p, p)) {
		p = other;
	}
}

void GlyphDistanceField::_Compute(_Grid grid) {
	for (unsigned y = 0; y < _height; y++) {
		for (unsigned x = 0; x < _width; x++) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, -1, 0);
			_Handle(grid, p, x, y, 0, -1);
			_Handle(grid, p, x, y, -1, -1);
			_Handle(grid, p, x, y, 1, -1);
			grid[y][x] = p;
		}

		for (int x = _width - 1; x >= 0; x--) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, 1, 0);
			grid[y][x] = p;
		}
	}

	for (int y = _height - 1; y >= 0; y--) {
		for (int x = _width - 1; x >= 0; x--) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, 1, 0);
			_Handle(grid, p, x, y, 0, 1);
			_Handle(grid, p, x, y, -1, 1);
			_Handle(grid, p, x, y, 1, 1);
			grid[y][x] = p;
		}

		for (unsigned x = 0; x < _width; x++) {
			vec2 p = grid[y][x];
			_Handle(grid, p, x, y, -1, 0);
			grid[y][x] = p;
		}
	}
}

void GlyphDistanceField::_Convert() {
	for (unsigned y = 0; y < _height; y++) {
		for (unsigned x = 0; x < _width; x++) {
			float g1Length = glm::length(_grid1[y][x]);
			float g2Length = glm::length(_grid2[y][x]);
			_data[x + _width * y] = { std::max(0.0f, g2Length - g1Length), 0, 0, 0 };
		}
	}
}

}
