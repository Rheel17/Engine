/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "GlyphBuffer.h"

#include "Encoding.h"

namespace rheel {

GlyphBuffer::callback_lru::callback_lru(GlyphBuffer& gb) :
		_gb(gb) {}

uintptr_t GlyphBuffer::callback_lru::MakeSpace() {
	uintptr_t removePtr = least_recently_used_policy::MakeSpace();
	char32_t remove = *reinterpret_cast<char32_t*>(removePtr);

	_gb._next_slot = _gb._glyph_slot[remove];
	_gb._glyphs[_gb._next_slot] = 0;
	_gb._glyph_slot.erase(remove);
	_gb._cache_size--;

	return removePtr;
}

GlyphBuffer::GlyphBuffer(Font& font, unsigned samples) :
		_font(font) {

	// pre-allocate a 256k glyph buffer.
	_glyph_buffer.SetDataEmptySize(262144, gl::Buffer::Usage::STREAM_DRAW);

	// create the vao
	_glyph_vao.SetVertexAttributes<vec3>(_glyph_buffer);
	_glyph_vao.SetVertexAttributes<vec4>(_transform_buffer, 0, samples);
}

GlyphBuffer::~GlyphBuffer() {
	free(_glyph_memory);
}

size_t GlyphBuffer::Load(const char* text) {
	size_t loaded = 0;
	size_t updates = 0;

	while (updates < CAPACITY) {
		char32_t character = Encoding::Utf8ToCodePoint(text);
		if (character == 0) {
			break;
		}

		if (Load_(character)) {
			updates++;
		}

		text += Encoding::Utf8Lenght(character);
		loaded++;
	}

	if (updates > 0) {
		// the buffer changed: update the glyph and index buffer
		_glyph_buffer.SetData(_glyph_memory, _glyph_memory_count, gl::Buffer::Usage::STREAM_DRAW);
		UpdateIndices_();
	}

	return loaded;
}

const Glyph& GlyphBuffer::GetLoadedGlyph(char32_t c) {
	return _cache.Get(c);
}

gl::Buffer& GlyphBuffer::GetTransformBuffer() {
	return _transform_buffer;
}

const gl::VertexArray& GlyphBuffer::GetVertexArray() const {
	return _glyph_vao;
}

const std::pair<unsigned, unsigned>& GlyphBuffer::GetOffset(char32_t character) const {
	return _glyph_index_offsets.find(character)->second;
}

bool GlyphBuffer::Load_(char32_t character) {
	if (!_cache.Put(character, [this](char32_t character) {
		return _font.LoadCharacter(character);
	})) {
		return false;
	}

	// the character was put newly in the buffer
	assert(_next_slot != _no_slot);
	assert(_next_slot < CAPACITY);
	Glyph& glyph = _cache.Get(character);

	_glyphs[_next_slot] = character;
	_glyph_slot[character] = _next_slot;
	_cache_size++;

	// actually load the glyph data into the buffer
	auto[offset, prevLength] = _glyph_offsets[_next_slot];
	size_t newLength = GetGlyphLength_(glyph);

	if (offset == 0 && _next_slot > 0) {
		auto[beforeOffset, beforeLength] = _glyph_offsets[_next_slot - 1];
		offset = beforeOffset + beforeLength;
	}

	if (newLength != prevLength) {
		// reallocate the glyph data array and move values past the current slot
		// to their new location.
		ptrdiff_t diff = static_cast<ptrdiff_t>(newLength) - static_cast<ptrdiff_t>(prevLength);
		_glyph_memory_count += diff;

		if (_glyph_memory_capacity < _glyph_memory_count) {
			_glyph_memory_capacity = _glyph_memory_count;
			_glyph_memory = static_cast<GlyphDataType*>(std::realloc(_glyph_memory, _glyph_memory_capacity * sizeof(GlyphDataType)));
		}

		std::memmove(_glyph_memory + offset + newLength, _glyph_memory + offset + prevLength,
				(_glyph_memory_count - offset - newLength) * sizeof(GlyphDataType));

		// update the offsets
		for (size_t i = _next_slot + 1; i < _cache_size; i++) {
			_glyph_offsets[i].first += diff;
		}
	}

	LoadGlyph_(glyph, _glyph_memory + offset);
	_glyph_offsets[_next_slot] = std::make_pair(offset, newLength);

	// update the next slot
	if (_cache_size == CAPACITY) {
		_next_slot = _no_slot;
	} else {
		_next_slot++;
	}

	return true;
}

void GlyphBuffer::UpdateIndices_() {
	std::vector<unsigned> indices;
	_glyph_index_offsets.clear();

	unsigned offset = 0;

	for (size_t slot = 0; slot < _cache_size; slot++) {
		char32_t c = _glyphs[slot];
		if (c == 0) {
			continue;
		}

		AddIndices_(c, indices);
		unsigned count = indices.size() - offset;

		_glyph_index_offsets[c] = std::make_pair(offset, count);
		offset = indices.size();
	}

	_glyph_vao.SetVertexIndices(indices);
}

void GlyphBuffer::AddIndices_(char32_t character, std::vector<uint32_t>& indices) {
	auto[offset, length] = _glyph_offsets.find(_glyph_slot.find(character)->second)->second;
	const Glyph& glyph = _cache.Get(character);

	size_t triangleCount = glyph.Triangles().size();
	size_t bezierCount = glyph.BezierCurveTriangles().size();

	// triangles
	for (size_t i = 0; i < triangleCount; i++) {
		indices.push_back(offset);
		indices.push_back(offset + 1 + i * 2);
		indices.push_back(offset + 2 + i * 2);
	}

	// bezier curves
	uint32_t bezierOffset = offset;

	if (triangleCount > 0) {
		bezierOffset += 1 + 2 * triangleCount;
	}

	for (size_t i = 0; i < 3 * bezierCount; i++) {
		indices.push_back(bezierOffset + i);
	}
}

size_t GlyphBuffer::GetGlyphLength_(const Glyph& glyph) {
	size_t length = 3 * glyph.BezierCurveTriangles().size();

	if (glyph.Triangles().size() > 0) {
		length += 1 + 2 * glyph.Triangles().size();
	}

	return length;
}

void GlyphBuffer::LoadGlyph_(const Glyph& glyph, GlyphBuffer::GlyphDataType* array) {
	// common point for the triangles
	if (glyph.Triangles().size() > 0) {
		array[0] = glyph.Triangles()[0][0];
		array++;
	}

	// other point for triangles
	for (const auto& triangle : glyph.Triangles()) {
		array[0] = triangle[1];
		array[1] = triangle[2];

		array += 2;
	}

	// bezier curves
	for (const auto& triangle : glyph.BezierCurveTriangles()) {
		array[0] = triangle[0];
		array[1] = triangle[1];
		array[2] = triangle[2];

		array += 3;
	}
}

}