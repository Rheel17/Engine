#ifndef MODEL_H_
#define MODEL_H_
#include "_common.h"

#include <vector>

namespace rheel {

class RE_API Model {
	friend class ColladaParser;

public:
	enum ModelFormat {
		FormatCollada
	};

	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texture;
	};

public:
	Model(const std::string& filename, ModelFormat format);

	const std::vector<Vertex>& Vertices() const;
	const std::vector<unsigned>& Indices() const;

private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned> _indices;

};

}

#endif
