/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_DRAWINDIRECTBUFFER_H
#define RHEELENGINE_DRAWINDIRECTBUFFER_H
#include "../../_common.h"

#include "Buffer.h"

namespace rheel::gl {

struct RE_API draw_arrays_indirect_command {
	GLuint count;
	GLuint instance_count;
	GLuint first;
	GLuint base_instance;
};

struct RE_API draw_elements_indirect_command {
	GLuint count;
	GLuint instance_count;
	GLuint first_index;
	GLuint base_vertex;
	GLuint base_instance;
};

template<class CommandType>
class DrawIndirectBuffer : public Buffer {
	static_assert(std::is_same_v<CommandType, draw_arrays_indirect_command> || std::is_same_v<CommandType, draw_elements_indirect_command>,
			"Command must be either draw_arrays_indirect_command or draw_elements_indirect_command");

public:
	using Command = CommandType;

	DrawIndirectBuffer() :
			Buffer(Target::DRAW_INDIRECT) {}

};

using DrawArraysIndirectBuffer = DrawIndirectBuffer<draw_arrays_indirect_command>;
using DrawElementsIndirectBuffer = DrawIndirectBuffer<draw_elements_indirect_command>;

}

#endif
