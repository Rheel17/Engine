/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef RHEELENGINE_POSTPROCESSINGSTACK_H
#define RHEELENGINE_POSTPROCESSINGSTACK_H
#include "../_common.h"

#include <array>

#include "PostProcessing/Bloom.h"

namespace rheel {

class RE_API PostProcessingStack {
	// TODO: make PostProcessingStack movable and copyable
	RE_NO_MOVE(PostProcessingStack);
	RE_NO_COPY(PostProcessingStack);

	friend class PostProcessingEffect;

public:
	PostProcessingStack();

	/**
	 * Renders all post-processing effects, with the input being a framebuffer
	 * as outputted by a SceneRenderer. The final scene will be drawn to the
	 * quad at pos with dimensions size.
	 */
	void Render(const gl::Framebuffer& input, const ivec2& pos, const ivec2& size) const;

	/**
	 * Enables the bloom effect.
	 */
	void SetBloom(Bloom bloom);

	/**
	 * Disables the bloom effect.
	 */
	void ClearBloom();

private:
	const gl::Framebuffer& _resolve_input(const gl::Framebuffer& input) const;

	unsigned _unused_framebuffer_index() const;
	unsigned _get_framebuffer_index(const gl::Framebuffer& buffer) const;
	gl::Framebuffer& _framebuffer(unsigned index) const;
	void _mark_framebuffer_use(unsigned index, bool flag) const;

	std::optional<Bloom> _bloom;

	mutable unsigned _width = 0;
	mutable unsigned _height = 0;
	mutable std::array<std::pair<gl::Framebuffer, bool>, 3> _temp_buffers;

};

}

#endif
