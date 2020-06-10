/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ENUMS_H
#define RHEELENGINE_ENUMS_H
#include "../../_common.h"

#include <GL/glwr.h>

namespace rheel::gl {

enum class Type {
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE, BYTE = GL_BYTE, UNSIGNED_SHORT = GL_UNSIGNED_SHORT, SHORT = GL_SHORT,
	UNSIGNED_INT = GL_UNSIGNED_INT, INT = GL_INT, HALF_FLOAT = GL_HALF_FLOAT, FLOAT = GL_FLOAT,
	DOUBLE = GL_DOUBLE, FIXED = GL_FIXED, INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
	UNSIGNED_BYTE_3_3_2 = GL_UNSIGNED_BYTE_3_3_2, UNSIGNED_BYTE_2_3_3_REV = GL_UNSIGNED_BYTE_2_3_3_REV,
	UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5, UNSIGNED_SHORT_5_6_5_REV = GL_UNSIGNED_SHORT_5_6_5_REV,
	UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4, UNSIGNED_SHORT_4_4_4_4_REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1, UNSIGNED_SHORT_1_5_5_5_REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	UNSIGNED_INT_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8, UNSIGNED_INT_8_8_8_8_REV = GL_UNSIGNED_INT_8_8_8_8_REV,
	UNSIGNED_INT_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2, UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
	UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
};

enum class Format {
	RED = GL_RED, RG = GL_RG, RGB = GL_RGB, BGR = GL_BGR, RGBA = GL_RGBA, BGRA = GL_BGRA, RED_INTEGER = GL_RED_INTEGER,
	RG_INTEGER = GL_RG_INTEGER, RGB_INTEGER = GL_RGB_INTEGER, BGR_INTEGER = GL_BGR_INTEGER,
	RGBA_INTEGER = GL_RGBA_INTEGER, BGRA_INTEGER = GL_BGRA_INTEGER, STENCIL_INDEX = GL_STENCIL_INDEX,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT, DEPTH_STENCIL = GL_DEPTH_STENCIL
};

enum class InternalFormat {
	// base internal formats
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT, DEPTH_STENCIL = GL_DEPTH_STENCIL,
	RED = GL_RED, RG = GL_RG, RGB = GL_RGB, RGBA = GL_RGBA,

	// sized internal formats
	R8 = GL_R8, R8_SNORM = GL_R8_SNORM, R16 = GL_R16, R16_SNORM = GL_R16_SNORM, RG8 = GL_RG8, RG8_SNORM = GL_RG8_SNORM,
	RG16 = GL_RG16, RG16_SNORM = GL_RG16_SNORM, R3_G3_B2 = GL_R3_G3_B2, RGB4 = GL_RGB4, RGB5 = GL_RGB5, RGB8 = GL_RGB8,
	RGB8_SNORM = GL_RGB8_SNORM, RGB10 = GL_RGB10, RGB12 = GL_RGB12, RGB16_SNORM = GL_RGB16_SNORM, RGBA2 = GL_RGBA2,
	RGBA4 = GL_RGBA4, RGB5_A1 = GL_RGB5_A1, RGBA8 = GL_RGBA8, RGBA8_SNORM = GL_RGBA8_SNORM, RGB10_A2 = GL_RGB10_A2,
	RGB10_A2UI = GL_RGB10_A2UI, RGBA12 = GL_RGBA12, RGBA16 = GL_RGBA16, SRGB8 = GL_SRGB8, SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
	R16F = GL_R16F, RG16F = GL_RG16F, RGB16F = GL_RGB16F, RGBA16F = GL_RGBA16F, R32F = GL_R32F, RG32F = GL_RG32F,
	RGB32F = GL_RGB32F, RGBA32F = GL_RGBA32F, R11F_G11F_B10F = GL_R11F_G11F_B10F, RGB9_E5 = GL_RGB9_E5, R8I = GL_R8I,
	R8UI = GL_R8UI, R16I = GL_R16I, R16UI = GL_R16UI, R32I = GL_R32I, R32UI = GL_R32UI, RG8I = GL_RG8I, RG8UI = GL_RG8UI,
	RG16I = GL_RG16I, RG16UI = GL_RG16UI, RG32I = GL_RG32I, RG32UI = GL_RG32UI, RGB8I = GL_RGB8I, RGB8UI = GL_RGB8UI,
	RGB16I = GL_RGB16I, RGB16UI = GL_RGB16UI,  RGB32I = GL_RGB32I, RGB32UI = GL_RGB32UI, RGBA8I = GL_RGBA8I,
	RGBA8UI = GL_RGBA8UI, RGBA16I = GL_RGBA16I, RGBA16UI = GL_RGBA16UI, RGBA32I = GL_RGBA32I, RGBA32UI = GL_RGBA32UI,

	// compressed internal formats
	COMPRESSED_RED = GL_COMPRESSED_RED, COMPRESSED_RG = GL_COMPRESSED_RG, COMPRESSED_RGB = GL_COMPRESSED_RGB,
	COMPRESSED_RGBA = GL_COMPRESSED_RGBA, COMPRESSED_SRGB = GL_COMPRESSED_SRGB,
	COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA, COMPRESSED_RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
	COMPRESSED_SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1, COMPRESSED_RG_RGTC1 = GL_COMPRESSED_RG_RGTC2,
	COMPRESSED_SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
	COMPRESSED_RGBA_BPTC_UNORM = GL_COMPRESSED_RGBA_BPTC_UNORM,
	COMPRESSED_SRGB_ALPHA_BPTC_UNORM = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
	COMPRESSED_RGB_BPTC_SIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
	COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,

	// formats not in a table
	DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16, DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
	DEPTH_COMPONENT_32F = GL_DEPTH_COMPONENT32F, DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
	DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8, STENCIL_INDEX8 = GL_STENCIL_INDEX8
};

enum class BlendFactor {
	ZERO = GL_ZERO, ONE = GL_ONE,
	SRC_COLOR = GL_SRC_COLOR, ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
	SRC_ALPHA = GL_SRC_ALPHA, ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	DST_COLOR = GL_DST_COLOR, ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	DST_ALPHA = GL_DST_ALPHA, ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
	SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
};

enum class LogicOp {
	CLEAR = GL_CLEAR, SET = GL_SET,
	COPY = GL_COPY,	COPY_INVERTED = GL_COPY_INVERTED,
	NOOP = GL_NOOP, INVERT = GL_INVERT, AND = GL_AND, NAND = GL_NAND,
	OR = GL_OR, NOR = GL_NOR, XOR = GL_XOR, EQUIV = GL_EQUIV,
	AND_REVERSE = GL_AND_REVERSE, AND_INVERTED = GL_AND_INVERTED,
	OR_REVERSE = GL_OR_REVERSE, OR_INVERTED = GL_OR_INVERTED,
};

enum class CompareFunction {
	LEQUAL   = GL_LEQUAL,
	GEQUAL   = GL_GEQUAL,
	LESS     = GL_LESS,
	GREATER  = GL_GREATER,
	EQUAL    = GL_EQUAL,
	NOTEQUAL = GL_NOTEQUAL,
	ALWAYS   = GL_ALWAYS,
	NEVER    = GL_NEVER
};

enum class CullFace {
	FRONT = GL_FRONT, BACK = GL_BACK, FRONT_AND_BACK = GL_FRONT_AND_BACK
};

enum class StencilFunction {
	KEEP      = GL_KEEP,
	ZERO      = GL_ZERO,
	REPLACE   = GL_REPLACE,
	INCR      = GL_INCR,
	INCR_WRAP = GL_INCR_WRAP,
	DECR      = GL_DECR_WRAP,
	DECR_WRAP = GL_DECR_WRAP,
	INVERT    = GL_INVERT
};

}

#endif
