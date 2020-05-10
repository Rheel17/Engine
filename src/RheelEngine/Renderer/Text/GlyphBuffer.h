/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_GLYPHBUFFER_H
#define RHEELENGINE_GLYPHBUFFER_H
#include "../../_common.h"

#include "Font.h"
#include "Glyph.h"
#include "../OpenGL/Buffer.h"
#include "../../Util/Cache.h"

namespace rheel {

class RE_API GlyphBuffer {
	RE_NO_COPY(GlyphBuffer);
	RE_NO_MOVE(GlyphBuffer);

	struct callback_lru : public least_recently_used_policy {
		callback_lru(GlyphBuffer& gb);

		virtual uintptr_t EnsureSpace() override;

	private:
		GlyphBuffer& _gb;

	};

public:
	GlyphBuffer(Font& font);
	~GlyphBuffer();

	/**
	 * Loads an UTF-8 string into the buffer. Returns the number of characters
	 * that were put in the buffer.
	 */
	size_t Load(const char* text);

	/**
	 * Returns the GPU buffer.
	 */
	const gl::Buffer& GetGlyphBuffer() const;

	/**
	 * Returns the [position, count] of the loaded glyph. If the specified
	 * character is not loaded in the glyph buffer using the last Load(...)
	 * call, calling this method causes undefined behaviour.
	 */
	const std::pair<size_t, size_t>& GetOffset(char32_t character) const;

public:
	static constexpr size_t CAPACITY = 256;

private:
	bool Load_(char32_t character);

	Font& _font;

	Cache<char32_t, Glyph, callback_lru> _cache{ CAPACITY, callback_lru(*this) };
	std::array<Glyph*, CAPACITY> _glyphs{};
	std::unordered_map<char32_t, size_t> _glyph_index;
	std::unordered_map<size_t, std::pair<size_t, size_t>> _glyph_offsets;
	size_t _next_slot = 0;
	size_t _cache_size = 0;

	// raw memory pointer in which all glyph vertex data is stored.
	using GlyphDataType = vec3;
	static_assert(std::is_trivially_copyable_v<GlyphDataType>);

	GlyphDataType* _glyph_memory = nullptr;
	size_t _glyph_memory_count = 0;

	gl::Buffer _glyph_buffer{ gl::Buffer::Target::ARRAY };

private:
	static constexpr size_t _no_slot = std::numeric_limits<size_t>::max();
	static_assert(_no_slot > CAPACITY);

	static size_t GetGlyphLength_(const Glyph& glyph);
	static void LoadGlyph_(const Glyph& glyph, GlyphDataType* array);
};

}

#endif
