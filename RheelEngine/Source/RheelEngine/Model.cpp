#include "Model.h"

#include <iostream>
#include <fstream>

#include "ColladaParser.h"

namespace rheel {

Model::Model(const std::string& filename, ModelFormat format) {
	switch (format) {
		case FormatCollada:
			ColladaParser::ParseCollada(*this, filename);
			break;
	}
}

const std::vector<Model::Vertex>& Model::Vertices() const {
	return _vertices;
}

const std::vector<unsigned>& Model::Indices() const {
	return _indices;
}

}
