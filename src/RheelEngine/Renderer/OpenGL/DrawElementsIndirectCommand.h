/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_DRAWELEMENTSINDIRECTCOMMAND_H
#define RHEELENGINE_DRAWELEMENTSINDIRECTCOMMAND_H
#include "../../_common.h"

#include <GL/glew.h>

struct RE_API draw_elements_indirect_command {
	GLuint count;
	GLuint instance_count;
	GLuint first_index;
	GLuint base_vertex;
	GLuint base_instance;
};

#endif