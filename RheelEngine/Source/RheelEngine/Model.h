#ifndef MODEL_H_
#define MODEL_H_
#include "_common.h"

#include <memory>
#include <vector>
#include <functional>

namespace rheel {

class Model;
using ModelPtr = std::shared_ptr<Model>;

class RE_API Model {
	friend class ColladaParser;

public:
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texture;
	};

public:
	const std::vector<Vertex>& Vertices() const;
	const std::vector<unsigned>& Indices() const;

private:
	Model() = default;

	std::vector<Vertex> _vertices;
	std::vector<unsigned> _indices;

public:
	static ModelPtr LoadCollada(const std::string& filename);

};

}

#endif
