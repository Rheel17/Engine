/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GLYPHBUFFER_H
#define RHEELENGINE_GLYPHBUFFER_H
#include "../../_common.h"

#include "Font.h"
#include "Glyph.h"
#include "../OpenGL/VertexArray.h"
#include "../../Util/Cache.h"

namespace rheel {

class RE_API GlyphBuffer {
	RE_NO_COPY(GlyphBuffer);
	RE_NO_MOVE(GlyphBuffer);

	struct callback_lru : public least_recently_used_policy {
		callback_lru(GlyphBuffer& gb);

		virtual uintptr_t MakeSpace() override;

	private:
		GlyphBuffer& _gb;

	};

public:
	GlyphBuffer(Font& font, unsigned samples);
	~GlyphBuffer();

	/**
	 * Loads an UTF-8 string into the buffer. Returns the number of characters
	 * that were put in the buffer.
	 */
	size_t Load(const char* text);

	/**
	 * Returns a loaded glyph for the character. If the specified character was
	 * not loaded in the glyph buffer using the last Load(...) call, calling
	 * this method will fail.
	 */
	const Glyph& GetLoadedGlyph(char32_t c);

	/**
	 * Returns the buffer for transforms.
	 */
	gl::Buffer& GetTransformBuffer();

	/**
	 * Returns the VAO
	 */
	const gl::VertexArray& GetVertexArray() const;

	/**
	 * Returns the [position, count] of the loaded glyph in the index buffer. If
	 * the specified character was not loaded in the glyph buffer using the last
	 * Load(...) call, calling this method will fail.
	 */
	const std::pair<unsigned, unsigned>& GetOffset(char32_t character) const;

public:
	static constexpr size_t CAPACITY = 256;

private:
	bool Load_(char32_t character);
	void UpdateIndices_();
	void AddIndices_(char32_t character, std::vector<uint32_t>& indices);

	Font& _font;

	Cache<char32_t, Glyph, callback_lru> _cache{ CAPACITY, callback_lru(*this) };
	std::array<char32_t, CAPACITY> _glyphs;
	std::unordered_map<char32_t, size_t> _glyph_slot;
	std::unordered_map<size_t, std::pair<size_t, size_t>> _glyph_offsets;
	std::unordered_map<char32_t, std::pair<unsigned, unsigned>> _glyph_index_offsets;
	size_t _next_slot = 0;
	size_t _cache_size = 0;

	// raw memory pointer in which all glyph vertex data is stored.
	using GlyphDataType = vec3;
	static_assert(std::is_trivially_copyable_v<GlyphDataType>);

	GlyphDataType* _glyph_memory = nullptr;
	size_t _glyph_memory_count = 0;
	size_t _glyph_memory_capacity = 0;

	gl::Buffer _glyph_buffer{ gl::Buffer::Target::ARRAY };
	gl::Buffer _transform_buffer{ gl::Buffer::Target::ARRAY };
	gl::VertexArray _glyph_vao;

private:
	static constexpr size_t _no_slot = std::numeric_limits<size_t>::max();
	static_assert(_no_slot > CAPACITY);

	static size_t GetGlyphLength_(const Glyph& glyph);
	static void LoadGlyph_(const Glyph& glyph, GlyphDataType* array);
};

}

#endif
