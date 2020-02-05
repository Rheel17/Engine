/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#include "VoxelLoader.h"

#include <fstream>

namespace rheel {

///////////////////////////////
// STREAM WRAPPER FOR VECTOR //
///////////////////////////////

template<typename CharT, typename TraitsT = std::char_traits<CharT> >
class VectorStream : public std::basic_streambuf<CharT, TraitsT> {

public:
	explicit VectorStream(std::vector<std::istream::char_type>& vec) {
		this->setg(vec.data(), vec.data(), vec.data() + vec.size());
	}

};

//////////////////////////
// GENERAL RIFF PARSING //
//////////////////////////

using uint = uint32_t;

/*
 * Read the next count bytes into data. If an error occurred, or the stream reached
 * end of file before the bytes could be completely read, an error will be shown and
 * the program will abort.
 */
static void checkRead(std::istream& input, char *data, size_t count) {
	input.read(data, count);

	if (!input.good() || input.gcount() != count) {
		Log::Error() << "Failed reading .vox file: an error occurred while trying to read " << count << " bytes" << std::endl;
		abort();
	}
}

/*
 * Read a little-endian unsigned integer.
 */
static uint readInt(std::istream& input) {
	char bytes[4];
	checkRead(input, bytes, 4);

	auto *ubytes = reinterpret_cast<uint8_t *>(bytes);
	uint part0 = uint(ubytes[0]);
	uint part1 = uint(ubytes[1]) << 8;
	uint part2 = uint(ubytes[2]) << 16;
	uint part3 = uint(ubytes[3]) << 24;

	return part0 | part1 | part2 | part3;
}

class RIFF {

public:
	using Byte = char;

	class Chunk {
		friend class RIFF;

	public:
		/*
		 * Read an arbitrary chunk
		 */
		explicit Chunk(std::istream& input) {
			// read the chunk header
			checkRead(input, _chunk_id, 4);
			_bytes_chunk = readInt(input);
			_bytes_children = readInt(input);

			// read chunk data
			_data.resize(_bytes_chunk);
			checkRead(input, _data.data(), _bytes_chunk);

			// read children
			int64_t childBytesLeft = _bytes_children;

			while (childBytesLeft > 0) {
				auto& child = _children.emplace_back(input);
				childBytesLeft -= child._bytes_chunk + child._bytes_children + 12;
			}
		}

		std::string GetID() const {
			return std::string(_chunk_id, 4);
		}

		VectorStream<Byte> GetDataStream() const {
			return VectorStream<Byte>(_data);
		}

		const std::vector<Chunk> & GetChildren() const {
			return _children;
		}

	private:
		Chunk() = default;

		char _chunk_id[4]{};
		uint _bytes_chunk = 0;
		uint _bytes_children = 0;

		mutable std::vector<Byte> _data{};
		std::vector<Chunk> _children{};

	};

public:
	/*
	 * Read a RIFF file.
	 */
	explicit RIFF(std::istream& input) {
		checkRead(input, _id, 4);
		_version = readInt(input);
		_root = Chunk(input);
	}

	std::string GetID() const {
		return std::string(_id, 4);
	}

	uint GetVersion() const {
		return _version;
	}

	const Chunk& GetRoot() const {
		return _root;
	}

private:
	char _id[4]{};
	uint _version;
	Chunk _root;

};

/////////////////////////
// ACTUAL .VOX READING //
/////////////////////////

VoxelImage VoxelLoader::_DoLoad(const std::string& path) {
	std::ifstream f(path, std::ios::binary);

	if (!f) {
		throw std::runtime_error("Error while reading image file: " + path);
	}

	return _LoadVOX(f);
}

VoxelImage VoxelLoader::_LoadVOX(std::istream& input) {
	// read the riff-formated file.
	RIFF riff(input);

	// check the file header
	if (riff.GetID() != "VOX " || riff.GetVersion() != 150) {
		Log::Error() << "Failed reading .vox file: file header invalid." << std::endl;
		abort();
	}

	const auto& main = riff.GetRoot();
	if (main.GetID() != "MAIN") {
		Log::Error() << "Failed reading .vox file: invalid chunk." << std::endl;
		abort();
	}

	if (main.GetChildren().empty()) {
		Log::Error() << "Failed reading .vox file: empty main child list." << std::endl;
		abort();
	}

	// currently we only accept a single model.
	// TODO: add support for multiple models
	if (main.GetChildren()[0].GetID() == "PACK") {
		Log::Error() << "Failed reading .vox file: multiple models via 'PACK' unsupported." << std::endl;
		abort();
	}

	// read the model. Note that XYZ in the engine is XZY in the .vox format.
	const auto& size = main.GetChildren()[0];
	auto sizeStreamProxy = size.GetDataStream();
	std::istream sizeStream(&sizeStreamProxy);

	unsigned width = readInt(sizeStream);
	unsigned depth = readInt(sizeStream);
	unsigned height = readInt(sizeStream);

	std::vector<Color> voxels;
	voxels.resize(width * height * depth);

	const auto& xyzi = main.GetChildren()[1];
	auto xyziStreamProxy = xyzi.GetDataStream();
	std::istream xyziStream(&xyziStreamProxy);

	uint voxelCount = readInt(xyziStream);
	std::array<std::vector<ivec3>, 256> rawVoxels;
	char data[4];

	for (uint i = 0; i < voxelCount; i++) {
		checkRead(xyziStream, data, 4);
		rawVoxels[data[3]].emplace_back(data[0], data[2], data[1]);
	}

	// read the rest: ignore the materials for now, search for the palette.
	// TODO: handle materials?
	std::array<Color, 256> palette{{}};
	palette[0] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (size_t i = 2; i < main.GetChildren().size(); i++) {
		const auto& rgba = main.GetChildren()[i];
		if (rgba.GetID() != "RGBA") {
			continue;
		}

		auto rgbaStreamProxy = rgba.GetDataStream();
		std::istream rgbaStream(&rgbaStreamProxy);

		for (int idx = 1; idx < 256; idx++) {
			checkRead(rgbaStream, data, 4);
			palette[idx] = { data[0] / 255.0f, data[1] / 255.0f, data[2] / 255.0f, data[3] / 255.0f };
		}

		break;
	}

	// push the voxels to the internal vector
	for (int i = 0; i < 256; i++) {
		Color c = palette[i];
		const std::vector<ivec3>& locations = rawVoxels[i];

		for (ivec3 v : locations) {
			voxels[v.x + v.y * width + v.z * width * height] = c;
		}
	}

	return VoxelImage(width, height, depth, std::move(voxels));
}

}
