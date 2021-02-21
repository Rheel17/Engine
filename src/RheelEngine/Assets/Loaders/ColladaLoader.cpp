/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ColladaLoader.h"

#include <cstring>

namespace rheel {

// TODO: we need a better collada loader

static void transform_up(vec3& vector, char up) {
	vec3 original = vector;

	switch (up) {
		case 'x': vector.x = -original.y;
			vector.y = original.x;
			break;
		case 'y': break;
		case 'z': vector.y = original.z;
			vector.z = -original.y;
			break;
		default: abort();
	}
}

ColladaLoader::Geometry::Geometry(XmlNode geometry) {
	XmlNode mesh = geometry.child("mesh");
	assert(mesh);

	// get the polygons of this mesh
	XmlNode polylist = mesh.child("polylist");
	assert(polylist);
	assert(!polylist.next_sibling("polylist"));

	// assert that all the polygons are of size 3
	XmlNode vcount = polylist.child("vcount");
	assert(vcount);

	std::vector<unsigned> vcount_list = _create_vector_unsigned(vcount);
	for (unsigned vcount_data : vcount_list) {
		if (vcount_data != 3) {
			Log::Error() << "Error loading collada file" << std::endl;
			abort();
		}
	}

	// create a map from source ids to sources
	std::unordered_map<std::string, XmlNode> sources;
	for (XmlNode source = mesh.child("source"); source; source = source.next_sibling("source")) {
		XmlAttribute id = source.attribute("id");
		assert(id);

		std::string idstr = "#" + std::string(id.value());
		sources[idstr] = source;
	}

	// read the inputs of the polylist, and store the sources in a map
	std::unordered_map<std::string, std::string> inputs;
	std::unordered_map<std::string, unsigned> offsets;

	for (XmlNode input = polylist.child("input"); input; input = input.next_sibling("input")) {
		XmlAttribute semantic = input.attribute("semantic");
		assert(semantic);

		XmlAttribute source = input.attribute("source");
		assert(source);

		XmlAttribute offset = input.attribute("offset");
		assert(offset);

		inputs[semantic.value()] = source.value();
		offsets[semantic.value()] = strtoul(offset.value(), nullptr, 10);
	}

	// read the vertices inputs
	std::unordered_map<std::string, std::string> vertices_inputs;

	XmlNode vertices = mesh.child("vertices");
	assert(vertices);

	XmlAttribute vertices_id = vertices.attribute("id");
	assert(vertices_id);

	const std::string vertices_id_string = "#" + std::string(vertices_id.value());

	for (XmlNode input = vertices.child("input"); input; input = input.next_sibling("input")) {
		XmlAttribute semantic = input.attribute("semantic");
		assert(semantic);

		XmlAttribute source = input.attribute("source");
		assert(source);

		vertices_inputs[semantic.value()] = source.value();
	}

	// get the attributes
	assert(inputs["VERTEX"] == vertices_id_string);
	std::vector<float> positions = ReadSource_(sources[vertices_inputs["POSITION"]]);
	std::vector<float> normals = ReadSource_(sources[inputs["NORMAL"]]);
	std::vector<float> texcoords = ReadSource_(sources[inputs["TEXCOORD"]]);

	auto extract_vec_3 = [](const std::vector<float>& v, std::size_t i) {
		return vec3(v[3 * i], v[3 * i + 1], v[3 * i + 2]);
	};

	unsigned position_offset = offsets["VERTEX"];
	unsigned normal_offset = offsets["NORMAL"];
	unsigned texcoord_offset = offsets["TEXCOORD"];

	// assemble the mesh
	XmlNode p = polylist.child("p");
	assert(p);
	auto p_list = _create_vector_unsigned(p);

	auto polylist_count = polylist.attribute("count");
	assert(polylist_count);
	unsigned count = strtoul(polylist_count.value(), nullptr, 10);

	unsigned stride = (p_list.size() / 3) / count;
	assert(p_list.size() % stride == 0);

	for (unsigned i = 0; i < p_list.size(); i += stride) {
		vec3 position = extract_vec_3(positions, p_list[i + position_offset]);
		vec3 normal = extract_vec_3(normals, p_list[i + normal_offset]);
		vec2 texcoord = extract_vec_3(texcoords, p_list[i + texcoord_offset]);

		model_vertex vertex{ position, normal, texcoord };

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

std::vector<float> ColladaLoader::Geometry::ReadSource_(XmlNode source) {
	XmlNode float_array = source.child("float_array");
	assert(float_array);

	XmlAttribute count = float_array.attribute("count");
	assert(count);

	return _create_vector_float(float_array, strtoul(count.value(), nullptr, 10));
}

void ColladaLoader::_parse_collada() const {
	// get the up vector
	XmlNode root = _xml_document->first_child();
	assert(root);

	XmlNode asset = root.child("asset");
	assert(asset);

	if (XmlNode up_axis = asset.child("up_axis")) {
		_up = tolower(up_axis.child_value()[0]);
	}

	// parse the geometries
	XmlNode library_geometries = root.child("library_geometries");
	assert(library_geometries);

	for (XmlNode geometry = library_geometries.child("geometry"); geometry; geometry = geometry.next_sibling("geometry")) {
		_parse_geometry(geometry);
	}

	// parse the scenes
	XmlNode library_visual_scenes = root.child("library_visual_scenes");
	assert(library_visual_scenes);

	for (XmlNode visual_scene = library_visual_scenes.child("visual_scene");
		 visual_scene;
		 visual_scene = visual_scene.next_sibling("visual_scene")) {
		_parse_scene(visual_scene);
	}
}

void ColladaLoader::_parse_geometry(XmlNode geometry) const {
	XmlAttribute id = geometry.attribute("id");
	assert(id);

	_geometries.insert({ "#" + std::string(id.value()), Geometry(geometry) });
}

void ColladaLoader::_parse_scene(XmlNode scene) const {
	for (XmlNode node = scene.child("node"); node; node = node.next_sibling("node")) {
		mat4 transform = glm::identity<mat4>();

		for (XmlNode matrix = node.child("matrix"); matrix; matrix = matrix.next_sibling("matrix")) {
			XmlAttribute matrixSid = matrix.attribute("sid");
			assert(matrixSid);

			if (std::string(matrixSid.value()) == "transform") {
				transform = _create_matrix(matrix);
				break;
			}
		}

		for (XmlNode instance_geometry = node.child("instance_geometry");
			 instance_geometry;
			 instance_geometry = instance_geometry.next_sibling("instance_geometry")) {
			XmlAttribute instance_geometry_url = instance_geometry.attribute("url");
			assert(instance_geometry_url);

			_add_geometry(_geometries[std::string(instance_geometry_url.value())], transform);
		}
	}
}

void ColladaLoader::_add_geometry(const Geometry& geometry, const mat4& transform) const {
	unsigned index_offset = _vertices.size();

	for (const model_vertex& vertex : geometry._vertices) {
		vec4 transformed_position = transform * vec4(vertex.position, 1.0f);
		vec3 transformed_position_3(transformed_position.x, transformed_position.y, transformed_position.z);
		model_vertex transformed = { transformed_position_3, vertex.normal, vertex.texture };

		transform_up(transformed.position, _up);
		transform_up(transformed.normal, _up);

		_vertices.push_back(transformed);
	}

	for (unsigned index : geometry._indices) {
		_indices.push_back(index_offset + index);
	}
}

Model ColladaLoader::Load(const std::string& path) const {
	_xml_document = std::make_unique<XmlDocument>();
	_xml_document->load_file(path.c_str());

	_parse_collada();
	Model model(std::move(_vertices), std::move(_indices));

	_xml_document.reset();

	_geometries = {};
	_vertices = {};
	_indices = {};

	return model;
}

std::vector<unsigned> ColladaLoader::_create_vector_unsigned(XmlNode node, int size) {
	// create the vector
	std::vector<unsigned> vec;

	// parse the values
	const char* values = node.child_value();
	const char* tmp = values;
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

std::vector<float> ColladaLoader::_create_vector_float(XmlNode node, int size) {
	// create the vector
	std::vector<float> vec;

	// parse the values
	const char* values = node.child_value();
	const char* tmp = values;
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

mat4 ColladaLoader::_create_matrix(XmlNode node) {
	std::vector<float> floats = _create_vector_float(node, 16);

	return mat4(
			floats[0], floats[4], floats[8], floats[12],
			floats[1], floats[5], floats[9], floats[13],
			floats[2], floats[6], floats[10], floats[14],
			floats[3], floats[7], floats[11], floats[15]);
}

}
