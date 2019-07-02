#include "Model.h"

#include <iostream>
#include <fstream>

#include "ColladaParser.h"

namespace rheel {

const std::vector<Model::Vertex>& Model::Vertices() const {
	return _vertices;
}

const std::vector<unsigned>& Model::Indices() const {
	return _indices;
}

ModelPtr Model::LoadCollada(const std::string& filename) {
	return ColladaParser::ParseCollada(filename);
}

}
