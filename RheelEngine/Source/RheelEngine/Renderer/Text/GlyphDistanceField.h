#ifndef GLYPHDISTANCEFIELD_H_
#define GLYPHDISTANCEFIELD_H_
#include "../../_common.h"

namespace rheel {

class RE_API GlyphDistanceField {
	RE_NO_COPY(GlyphDistanceField)
	RE_NO_MOVE(GlyphDistanceField)

	using _Grid = vec2 **;

public:
	GlyphDistanceField(unsigned char *data, unsigned width, unsigned height);
	~GlyphDistanceField();

	float *Data() const;

private:
	void _Handle(_Grid grid, vec2& p, unsigned x, unsigned y, int dx, int dy);
	void _Compute(_Grid grid);
	void _Convert();

	_Grid _grid1;
	_Grid _grid2;
	float *_data;
	unsigned _width;
	unsigned _height;

};

}

#endif
