/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef COLLADAPARSER_H_
#define COLLADAPARSER_H_
#include "../../_common.h"

#include <rapidxml_utils.hpp>

#include "Loader.h"
#include "../../Util/Hashes.h"
#include "../Model.h"

namespace rheel {

class RE_API ColladaLoader : public Loader<Model> {
	friend class AssetLoader;

	using _XmlFile = rapidxml::file<>;
	using _XmlDocument = rapidxml::xml_document<>;
	using _XmlNode = rapidxml::xml_node<>;
	using _XmlAttribute = rapidxml::xml_attribute<>;

private:
	class Geometry {
		friend class ColladaLoader;

	public:
		Geometry() = default;
		explicit Geometry(_XmlNode *type);

	private:
		struct _VertexHash {
			constexpr std::size_t operator()(const ModelVertex& v) const {
				return hash_all(v.position, v.position, v.texture);
			}
		};

		struct _VertexEqual {
			std::size_t operator()(const ModelVertex& v1, const ModelVertex& v2) const {
				return v1.position == v2.position && v1.normal == v2.normal && v1.texture == v2.texture;
			}
		};

		std::unordered_map<ModelVertex, unsigned, _VertexHash, _VertexEqual> _vertex_indices;
		std::vector<ModelVertex> _vertices{};
		std::vector<unsigned> _indices{};

	private:
		static std::vector<float> _ReadSource(_XmlNode *source);

	};

protected:
	Model _DoLoad(const std::string& path) const override;

private:
	ColladaLoader() = default;

	void ParseCOLLADA() const;
	void ParseGeometry(_XmlNode *geometry) const;
	void ParseScene(_XmlNode *scene) const;

	void AddGeometry(const Geometry& geometry, const mat4& transform) const;

	// TODO: clean up mutable mess
	mutable std::unique_ptr<_XmlFile> _xml_file;
	mutable std::unique_ptr<_XmlDocument> _xml_document;

	mutable std::unordered_map<std::string, Geometry> _geometries;
	mutable std::vector<ModelVertex> _vertices;
	mutable std::vector<unsigned> _indices;

	mutable char _up = 'y';

private:
	static std::vector<unsigned> _CreateVectorUnsigned(_XmlNode *node, int size = -1);
	static std::vector<float> _CreateVectorFloat(_XmlNode *node, int size = -1);
	static mat4 _CreateMatrix(_XmlNode *node);

};

}

#endif
