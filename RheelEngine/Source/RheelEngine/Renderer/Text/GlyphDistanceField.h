#ifndef GLYPHDISTANCEFIELD_H_
#define GLYPHDISTANCEFIELD_H_
#include "../../_common.h"

#include <vector>

namespace rheel {

class RE_API GlyphDistanceField {
	RE_NO_COPY(GlyphDistanceField)
	RE_NO_MOVE(GlyphDistanceField)

	using _Grid = vec2 **;

public:
	GlyphDistanceField(unsigned char *data, unsigned width, unsigned height);

	const float *Data() const;

private:
	void _Handle(_Grid grid, vec2& p, unsigned x, unsigned y, int dx, int dy);
	void _Compute(_Grid grid);
	void _Convert();

	_Grid _grid1;
	_Grid _grid2;
	std::vector<vec4> _data;
	unsigned _width;
	unsigned _height;
	unsigned _grid_width;
	unsigned _grid_height;

public:
	static constexpr int PADDING = 3;

};

}

#endif
