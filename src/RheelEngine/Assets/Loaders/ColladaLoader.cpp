/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "ColladaLoader.h"

#include <cstring>

namespace rheel {

// TODO: we need a better collada loader

static void transformUp(vec3& vector, char up) {
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

	std::vector<unsigned> vcountList = CreateVectorUnsigned_(vcount);
	for (unsigned vcountData : vcountList) {
		if (vcountData != 3) {
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
	std::unordered_map<std::string, std::string> verticesInputs;

	XmlNode vertices = mesh.child("vertices");
	assert(vertices);

	XmlAttribute verticesId = vertices.attribute("id");
	assert(verticesId);

	const std::string verticesIdString = "#" + std::string(verticesId.value());

	for (XmlNode input = vertices.child("input"); input; input = input.next_sibling("input")) {
		XmlAttribute semantic = input.attribute("semantic");
		assert(semantic);

		XmlAttribute source = input.attribute("source");
		assert(source);

		verticesInputs[semantic.value()] = source.value();
	}

	// get the attributes
	assert(inputs["VERTEX"] == verticesIdString);
	std::vector<float> positions = ReadSource_(sources[verticesInputs["POSITION"]]);
	std::vector<float> normals = ReadSource_(sources[inputs["NORMAL"]]);
	std::vector<float> texcoords = ReadSource_(sources[inputs["TEXCOORD"]]);

	auto extractVec3 = [](const std::vector<float>& v, std::size_t i) {
		return vec3(v[3 * i], v[3 * i + 1], v[3 * i + 2]);
	};

	unsigned positionOffset = offsets["VERTEX"];
	unsigned normalOffset = offsets["NORMAL"];
	unsigned texcoordOffset = offsets["TEXCOORD"];

	// assemble the mesh
	XmlNode p = polylist.child("p");
	assert(p);
	auto pList = CreateVectorUnsigned_(p);

	auto polylistCount = polylist.attribute("count");
	assert(polylistCount);
	unsigned count = strtoul(polylistCount.value(), nullptr, 10);

	unsigned stride = (pList.size() / 3) / count;
	assert(pList.size() % stride == 0);

	for (unsigned i = 0; i < pList.size(); i += stride) {
		vec3 position = extractVec3(positions, pList[i + positionOffset]);
		vec3 normal = extractVec3(normals, pList[i + normalOffset]);
		vec2 texcoord = extractVec3(texcoords, pList[i + texcoordOffset]);

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
	XmlNode floatArray = source.child("float_array");
	assert(floatArray);

	XmlAttribute count = floatArray.attribute("count");
	assert(count);

	return CreateVectorFloat_(floatArray, strtoul(count.value(), nullptr, 10));
}

void ColladaLoader::ParseCollada_() const {
	// get the up vector
	XmlNode root = _xml_document->first_child();
	assert(root);

	XmlNode asset = root.child("asset");
	assert(asset);

	if (XmlNode upAxis = asset.child("up_axis")) {
		_up = tolower(upAxis.child_value()[0]);
	}

	// parse the geometries
	XmlNode libraryGeometries = root.child("library_geometries");
	assert(libraryGeometries);

	for (XmlNode geometry = libraryGeometries.child("geometry"); geometry; geometry = geometry.next_sibling("geometry")) {
		ParseGeometry_(geometry);
	}

	// parse the scenes
	XmlNode libraryVisualScenes = root.child("library_visual_scenes");
	assert(libraryVisualScenes);

	for (XmlNode visualScene = libraryVisualScenes.child("visual_scene");
		 visualScene;
		 visualScene = visualScene.next_sibling("visual_scene")) {
		ParseScene_(visualScene);
	}
}

void ColladaLoader::ParseGeometry_(XmlNode geometry) const {
	XmlAttribute id = geometry.attribute("id");
	assert(id);

	_geometries.insert({ "#" + std::string(id.value()), Geometry(geometry) });
}

void ColladaLoader::ParseScene_(XmlNode scene) const {
	for (XmlNode node = scene.child("node"); node; node = node.next_sibling("node")) {
		mat4 transform = glm::identity<mat4>();

		for (XmlNode matrix = node.child("matrix"); matrix; matrix = matrix.next_sibling("matrix")) {
			XmlAttribute matrixSid = matrix.attribute("sid");
			assert(matrixSid);

			if (std::string(matrixSid.value()) == "transform") {
				transform = CreateMatrix_(matrix);
				break;
			}
		}

		for (XmlNode instanceGeometry = node.child("instance_geometry");
			 instanceGeometry;
			 instanceGeometry = instanceGeometry.next_sibling("instance_geometry")) {
			XmlAttribute instanceGeometryUrl = instanceGeometry.attribute("url");
			assert(instanceGeometryUrl);

			AddGeometry_(_geometries[std::string(instanceGeometryUrl.value())], transform);
		}
	}
}

void ColladaLoader::AddGeometry_(const Geometry& geometry, const mat4& transform) const {
	unsigned indexOffset = _vertices.size();

	for (const model_vertex& vertex : geometry._vertices) {
		vec4 transformedPosition = transform * vec4(vertex.position, 1.0f);
		vec3 transformedPosition3(transformedPosition.x, transformedPosition.y, transformedPosition.z);
		model_vertex transformed = { transformedPosition3, vertex.normal, vertex.texture };

		transformUp(transformed.position, _up);
		transformUp(transformed.normal, _up);

		_vertices.push_back(transformed);
	}

	for (unsigned index : geometry._indices) {
		_indices.push_back(indexOffset + index);
	}
}

Model ColladaLoader::Load(const std::string& path) const {
	_xml_document = std::make_unique<XmlDocument>();
	_xml_document->load_file(path.c_str());

	ParseCollada_();
	Model model(std::move(_vertices), std::move(_indices));

	_xml_document.reset();

	_geometries = {};
	_vertices = {};
	_indices = {};

	return model;
}

std::vector<unsigned> ColladaLoader::CreateVectorUnsigned_(XmlNode node, int size) {
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

std::vector<float> ColladaLoader::CreateVectorFloat_(XmlNode node, int size) {
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

mat4 ColladaLoader::CreateMatrix_(XmlNode node) {
	std::vector<float> floats = CreateVectorFloat_(node, 16);

	return mat4(
			floats[0], floats[4], floats[8], floats[12],
			floats[1], floats[5], floats[9], floats[13],
			floats[2], floats[6], floats[10], floats[14],
			floats[3], floats[7], floats[11], floats[15]);
}

}
