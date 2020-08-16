/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_COLLADALOADER_H
#define RHEELENGINE_COLLADALOADER_H
#include "../../_common.h"

#include <pugixml.hpp>

#include "Loader.h"
#include "../../Util/Hashes.h"
#include "../Model.h"

namespace rheel {

class RE_API ColladaLoader : public AbstractLoader<Model> {
	friend class AssetLoader;

	using XmlDocument = pugi::xml_document;
	using XmlNode = pugi::xml_node;
	using XmlAttribute = pugi::xml_attribute;

private:
	class Geometry {
		friend class ColladaLoader;

	public:
		Geometry() = default;
		explicit Geometry(XmlNode type);

	private:
		struct vertex_hash {
			constexpr std::size_t operator()(const model_vertex& v) const {
				return hashAll(v.position, v.position, v.texture);
			}
		};

		struct vertex_equal {
			std::size_t operator()(const model_vertex& v1, const model_vertex& v2) const {
				return v1.position == v2.position && v1.normal == v2.normal && v1.texture == v2.texture;
			}
		};

		std::unordered_map<model_vertex, unsigned, vertex_hash, vertex_equal> _vertex_indices;
		std::vector<model_vertex> _vertices{};
		std::vector<unsigned> _indices{};

	private:
		static std::vector<float> ReadSource_(XmlNode source);

	};

public:
	Model Load(const std::string& path) const override;

private:
	void ParseCollada_() const;
	void ParseGeometry_(XmlNode geometry) const;
	void ParseScene_(XmlNode scene) const;

	void AddGeometry_(const Geometry& geometry, const mat4& transform) const;

	// TODO: clean up mutable mess
	mutable std::unique_ptr<XmlDocument> _xml_document;

	mutable std::unordered_map<std::string, Geometry> _geometries;
	mutable std::vector<model_vertex> _vertices;
	mutable std::vector<unsigned> _indices;

	mutable char _up = 'y';

private:
	static std::vector<unsigned> CreateVectorUnsigned_(XmlNode node, int size = -1);
	static std::vector<float> CreateVectorFloat_(XmlNode node, int size = -1);
	static mat4 CreateMatrix_(XmlNode node);

};

}

#endif
