/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ColladaParser.h"

namespace rheel {

static void transformUp(vec3& vector, char up) {
	vec3 original = vector;

	switch (up) {
		case 'x':
			vector.x = -original.y;
			vector.y = original.x;
			break;
		case 'y':
			break;
		case 'z':
			vector.y = original.z;
			vector.z = -original.y;
			break;
	}
}

ColladaParser::Geometry::Geometry(_XmlNode *geometry) {
	_XmlNode *mesh = geometry->first_node("mesh");
	assert(mesh);

	// get the polygons of this mesh
	_XmlNode *polylist = mesh->first_node("polylist");
	assert(polylist);
	assert(!polylist->next_sibling("polylist"));

	// assert that all the polygons are of size 3
	_XmlNode *vcount = polylist->first_node("vcount");
	assert(vcount);

	std::vector<unsigned> vcount_list = _CreateVectorUnsigned(vcount);
	for (unsigned vcount_data : vcount_list) {
		assert(vcount_data == 3);
	}

	// create a map from source ids to sources
	std::unordered_map<std::string, _XmlNode *> sources;
	for (_XmlNode *source = mesh->first_node("source"); source; source = source->next_sibling("source")) {
		_XmlAttribute *id = source->first_attribute("id");
		assert(id);

		std::string idstr = "#" + std::string(id->value());
		sources[idstr] = source;
	}

	// read the inputs of the polylist, and store the sources in a map
	std::unordered_map<std::string, std::string> inputs;
	std::unordered_map<std::string, unsigned> offsets;

	for (_XmlNode *input = polylist->first_node("input"); input; input = input->next_sibling("input")) {
		_XmlAttribute *semantic = input->first_attribute("semantic");
		assert(semantic);

		_XmlAttribute *source = input->first_attribute("source");
		assert(source);

		_XmlAttribute *offset = input->first_attribute("offset");
		assert(offset);

		inputs[semantic->value()] = source->value();
		offsets[semantic->value()] = strtoul(offset->value(), nullptr, 10);
	}

	// read the vertices inputs
	std::unordered_map<std::string, std::string> verticesInputs;

	_XmlNode *vertices = mesh->first_node("vertices");
	assert(vertices);

	_XmlAttribute *vertices_id = vertices->first_attribute("id");
	assert(vertices_id);

	const std::string verticesID = "#" + std::string(vertices_id->value());

	for (_XmlNode *input = vertices->first_node("input"); input; input = input->next_sibling("input")) {
		_XmlAttribute *semantic = input->first_attribute("semantic");
		assert(semantic);

		_XmlAttribute *source = input->first_attribute("source");
		assert(source);

		verticesInputs[semantic->value()] = source->value();
	}

	// get the attributes
	assert(inputs["VERTEX"] == verticesID);
	std::vector<float> positions = _ReadSource(sources[verticesInputs["POSITION"]]);
	std::vector<float> normals = _ReadSource(sources[inputs["NORMAL"]]);
	std::vector<float> texcoords = _ReadSource(sources[inputs["TEXCOORD"]]);

	vec3 *vPositions = reinterpret_cast<vec3 *>(positions.data());
	vec3 *vNormals = reinterpret_cast<vec3 *>(normals.data());
	vec2 *vTexcoords = reinterpret_cast<vec2 *>(texcoords.data());

	unsigned positionOffset = offsets["VERTEX"];
	unsigned normalOffset = offsets["NORMAL"];
	unsigned texcoordOffset = offsets["TEXCOORD"];

	// assemble the mesh
	_XmlNode *p = polylist->first_node("p");
	assert(p);
	std::vector<unsigned> p_list = _CreateVectorUnsigned(p);

	_XmlAttribute *polylist_count = polylist->first_attribute("count");
	assert(polylist_count);
	unsigned count = strtoul(polylist_count->value(), nullptr, 10);

	unsigned stride = (p_list.size() / 3) / count;
	assert(p_list.size() % stride == 0);

	for (unsigned i = 0; i < p_list.size(); i += stride) {
		vec3 position = vPositions[p_list[i + positionOffset]];
		vec3 normal   = vNormals  [p_list[i + normalOffset  ]];
		vec2 texcoord = vTexcoords[p_list[i + texcoordOffset]];

		Model::Vertex vertex { position, normal, texcoord };

		auto iter = _vertex_indices.find(vertex);
		unsigned index;

		if (iter == _vertex_indices.end()) {
			index = _vertices.size();
			_vertices.push_back(vertex);
			_vertex_indices.insert({ vertex, index });
		} else {
			index = iter->second;
		}

		_indices.push_back(index);
	}
}

std::vector<float> ColladaParser::Geometry::_ReadSource(_XmlNode *source) {
	_XmlNode *float_array = source->first_node("float_array");
	assert(float_array);

	_XmlAttribute *count = float_array->first_attribute("count");
	assert(count);

	return _CreateVectorFloat(float_array, strtoul(count->value(), nullptr, 10));
}

ColladaParser::ColladaParser(const std::string& filename) {
	// create the document
	_xml_file = std::make_unique<_XmlFile>(filename.c_str());
	_xml_document = std::make_unique<_XmlDocument>();
	_xml_document->parse<0>(_xml_file->data());

	ParseCOLLADA();
}

void ColladaParser::ParseCOLLADA() {
	// get the up vector
	_XmlNode *root = _xml_document->first_node();
	assert(root);

	_XmlNode *asset = root->first_node("asset");
	assert(asset);

	if (_XmlNode *up_axis = asset->first_node("up_axis")) {
		_up = tolower(up_axis->value()[0]);
	}

	// parse the geometries
	_XmlNode *library_geometries = root->first_node("library_geometries");
	assert(library_geometries);

	for (_XmlNode *geometry = library_geometries->first_node("geometry"); geometry; geometry = geometry->next_sibling("geometry")) {
		ParseGeometry(geometry);
	}

	// parse the scenes
	_XmlNode *library_visual_scenes = root->first_node("library_visual_scenes");
	assert(library_visual_scenes);

	for (_XmlNode *visual_scene = library_visual_scenes->first_node("visual_scene"); visual_scene; visual_scene = visual_scene->next_sibling("visual_scene")) {
		ParseScene(visual_scene);
	}
}

void ColladaParser::ParseGeometry(_XmlNode *geometry) {
	_XmlAttribute *id = geometry->first_attribute("id");
	assert(id);

	_geometries.insert({ "#" + std::string(id->value()), Geometry(geometry) });
}

void ColladaParser::ParseScene(_XmlNode *scene) {
	for (_XmlNode *node = scene->first_node("node"); node; node = node->next_sibling("node")) {
		mat4 transform = glm::identity<mat4>();

		for (_XmlNode *matrix = node->first_node("matrix"); matrix; matrix = matrix->next_sibling("matrix")) {
			_XmlAttribute *matrix_sid = matrix->first_attribute("sid");
			assert(matrix_sid);

			if (std::string(matrix_sid->value()) == "transform") {
				transform = _CreateMatrix(matrix);
				break;
			}
		}

		for (_XmlNode *instance_geometry = node->first_node("instance_geometry"); instance_geometry; instance_geometry = instance_geometry->next_sibling("instance_geometry")) {
			_XmlAttribute *instance_geometry_url = instance_geometry->first_attribute("url");
			assert(instance_geometry_url);

			AddGeometry(_geometries[std::string(instance_geometry_url->value())], transform);
		}
	}
}

void ColladaParser::AddGeometry(const Geometry& geometry, const mat4& transform) {
	unsigned indexOffset = _vertices.size();

	for (const Model::Vertex& vertex : geometry._vertices) {
		vec4 transformedPosition = transform * vec4(vertex.position, 1.0f);
		Model::Vertex transformed = { transformedPosition, vertex.normal, vertex.texture };

		transformUp(transformed.position, _up);
		transformUp(transformed.normal, _up);

		_vertices.push_back(transformed);
	}

	for (unsigned index : geometry._indices) {
		_indices.push_back(indexOffset + index);
	}
}

void ColladaParser::ParseCollada(Model& model, const std::string& filename) {
	ColladaParser parser(filename);

	model._vertices = std::move(parser._vertices);
	model._indices = std::move(parser._indices);
}

std::vector<unsigned> ColladaParser::_CreateVectorUnsigned(_XmlNode *node, int size) {
	// create the vector
	std::vector<unsigned> vec;

	// parse the values
	const char *values = node->value();
	const char *tmp = values;
	do {
	    int l = strcspn(tmp, " ");

	    if (l > 0) {
	    	vec.push_back(strtoul(tmp, nullptr, 10));
	    }

	    tmp += l + 1;
	} while (tmp[-1]);

	// assert that we read everything correctly
	if (size >= 0) {
		assert(vec.size() == (unsigned) size);
	}

	// return the vector
	return vec;
}

std::vector<float> ColladaParser::_CreateVectorFloat(_XmlNode *node, int size) {
	// create the vector
	std::vector<float> vec;

	// parse the values
	const char *values = node->value();
	const char *tmp = values;
	do {
	    int l = strcspn(tmp, " ");

	    if (l > 0) {
	    	vec.push_back(strtof(tmp, nullptr));
	    }

	    tmp += l + 1;
	} while (tmp[-1]);

	// assert that we read everything correctly
	if (size >= 0) {
		assert(vec.size() == (unsigned) size);
	}

	// return the vector
	return vec;
}

mat4 ColladaParser::_CreateMatrix(_XmlNode *node) {
	std::vector<float> floats = _CreateVectorFloat(node, 16);

	return mat4(
			floats[0], floats[4],  floats[8], floats[12],
			floats[1], floats[5],  floats[9], floats[13],
			floats[2], floats[6], floats[10], floats[14],
			floats[3], floats[7], floats[11], floats[15]);
}

}
