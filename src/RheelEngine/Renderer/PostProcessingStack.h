/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef POSTPROCESSINGSTACK_H_
#define POSTPROCESSINGSTACK_H_
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
	void Render(const _GLFramebuffer& input, const ivec2& pos, const ivec2& size) const;

	/**
	 * Enables the bloom effect.
	 */
	void SetBloom(Bloom bloom);

	/**
	 * Disables the bloom effect.
	 */
	void ClearBloom();

private:
	const _GLFramebuffer& _ResolveInput(const _GLFramebuffer& input) const;

	unsigned _UnusedFramebufferIndex() const;
	unsigned _GetFramebufferIndex(const _GLFramebuffer& buffer) const;
	_GLFramebuffer& _Framebuffer(unsigned index) const;
	void _MarkFramebufferUse(unsigned index, bool flag) const;

	std::optional<Bloom> _bloom;

	mutable unsigned _width = 0;
	mutable unsigned _height = 0;
	mutable std::array<std::pair<_GLFramebuffer, bool>, 3> _temp_buffers;

};

}

#endif
