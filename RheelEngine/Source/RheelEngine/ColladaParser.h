#ifndef COLLADAPARSER_H_
#define COLLADAPARSER_H_
#include "_common.h"

#include <rapidxml_utils.hpp>

#include <unordered_set>

#include "Util/Hashes.h"
#include "Model.h"

namespace rheel {

class RE_API ColladaParser {

	using _XmlFile = rapidxml::file<>;
	using _XmlDocument = rapidxml::xml_document<>;
	using _XmlNode = rapidxml::xml_node<>;
	using _XmlAttribute = rapidxml::xml_attribute<>;

private:
	class Geometry {
		friend class ColladaParser;

	public:
		Geometry() = default;
		Geometry(_XmlNode *type);

	private:
		struct _VertexHash {
			constexpr std::size_t operator()(const Model::Vertex& v) const {
				return hash_all(v.position, v.position, v.texture);
			}
		};

		struct _VertexEqual {
			std::size_t operator()(const Model::Vertex& v1, const Model::Vertex& v2) const {
				return v1.position == v2.position && v1.normal == v2.normal && v1.texture == v2.texture;
			}
		};

		std::unordered_map<Model::Vertex, unsigned, _VertexHash, _VertexEqual> _vertex_indices;
		std::vector<Model::Vertex> _vertices;
		std::vector<unsigned> _indices;

	private:
		static std::vector<float> _ReadSource(_XmlNode *source);

	};

private:
	ColladaParser(const std::string& filename);

	void ParseCOLLADA();
	void ParseGeometry(_XmlNode *geometry);
	void ParseScene(_XmlNode *scene);

	void AddGeometry(const Geometry& geometry, const mat4& transform);

	std::shared_ptr<_XmlFile> _xml_file;
	std::shared_ptr<_XmlDocument> _xml_document;

	std::unordered_map<std::string, Geometry> _geometries;
	std::vector<Model::Vertex> _vertices;
	std::vector<unsigned> _indices;

	char _up = 'y';

public:
	static ModelPtr ParseCollada(const std::string& filename);

private:
	static std::vector<unsigned> _CreateVectorUnsigned(_XmlNode *node, int size = -1);
	static std::vector<float> _CreateVectorFloat(_XmlNode *node, int size = -1);
	static mat4 _CreateMatrix(_XmlNode *node);

};

}

#endif
