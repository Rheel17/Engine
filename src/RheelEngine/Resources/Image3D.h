/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef IMAGE3D_H_
#define IMAGE3D_H_
#include "../_common.h"

#include "../Color.h"

namespace rheel {

class RE_API Image3D {
	RE_NO_MOVE(Image3D);
	RE_NO_COPY(Image3D);

public:
	/**
	 * Creates an empty 3d image with the given dimensions
	 */
	Image3D(unsigned xSize, unsigned ySize, unsigned zSize);

	/**
	 * Creates the 3d image based on a voxel file in the file system.
	 */
	explicit Image3D(const std::string& filename);

	unsigned GetWidth() const;
	unsigned GetHeight() const;
	unsigned GetDepth() const;

	const Color& At(unsigned x, unsigned y, unsigned z) const;

private:
	Image3D() = default;

	void _LoadVOX(std::istream& input);

	void _LoadFromFile(const std::string& filename);
	void _LoadFromStream(std::istream& input);

	unsigned _width = 0;
	unsigned _height = 0;
	unsigned _depth = 0;

	std::vector<Color> _voxels;

};

}

#endif
