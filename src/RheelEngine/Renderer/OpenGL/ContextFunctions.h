/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_CONTEXTFUNCTIONS_H
#define RHEELENGINE_CONTEXTFUNCTIONS_H
#include "../../_common.h"

#include "Enums.h"

namespace rheel::gl {

class RE_API ContextFunctions {

public:
	ContextFunctions();
	explicit ContextFunctions(ContextFunctions* parent);

	// clear
	void SetClearColor(float red, float green, float blue, float alpha);

	// blending
	void SetBlendFunction(BlendFactor sfactor, BlendFactor dfactor);
	void SetBlendFunction(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha);

	// logic
	void SetLogicOp(LogicOp opcode);

	// depth
	void SetDepthFunction(CompareFunction func);

	// culling
	void SetCullFace(CullFace mode);

	// masks
	void SetColorMask(bool red, bool green, bool blue, bool alpha);
	void SetDepthMask(bool mask);

	// stencil
	void SetStencilFunc(CompareFunction func, uint8_t reference, uint8_t mask);
	void SetStencilMask(uint8_t mask);
	void SetStencilOp(StencilFunction sfail, StencilFunction dpfail, StencilFunction dppass);

	void ResetChanges();

private:
	std::tuple<float, float, float, float> GetClearColor_() const;
	std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor> GetBlendFunction_() const;
	LogicOp GetLogicOp_() const;
	CompareFunction GetDepthFunction_() const;
	CullFace GetCullFace_() const;
	std::tuple<bool, bool, bool, bool> GetColorMask_() const;
	bool GetDepthMask_() const;
	std::tuple<CompareFunction, uint8_t, uint8_t> GetStencilFunc_() const;
	uint8_t GetStencilMask_() const;
	std::tuple<StencilFunction, StencilFunction, StencilFunction> GetStencilOp_() const;

	std::optional<std::tuple<float, float, float, float>> _clear_color;
	std::optional<std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor>> _blending_factors;
	std::optional<LogicOp> _logic_op;
	std::optional<CompareFunction> _depth_function;
	std::optional<CullFace> _cull_face;
	std::optional<std::tuple<bool, bool, bool, bool>> _color_mask;
	std::optional<bool> _depth_mask;
	std::optional<std::tuple<CompareFunction, uint8_t, uint8_t>> _stencil_func;
	std::optional<uint8_t> _stencil_mask;
	std::optional<std::tuple<StencilFunction, StencilFunction, StencilFunction>> _stencil_op;

	ContextFunctions* _parent;

private:
	static constexpr std::tuple<float, float, float, float> _default_clear_color = { 0.0f, 0.0f, 0.0f, 0.0f };
	static constexpr std::tuple<BlendFactor, BlendFactor, BlendFactor, BlendFactor> _default_blending_factors =
			{ BlendFactor::ONE, BlendFactor::ZERO, BlendFactor::ONE, BlendFactor::ZERO };
	static constexpr LogicOp _default_logic_op = LogicOp::COPY;
	static constexpr CompareFunction _default_depth_function = CompareFunction::LESS;
	static constexpr CullFace _default_cull_face = CullFace::BACK;
	static constexpr std::tuple<bool, bool, bool, bool> _default_color_mask = { true, true, true, true };
	static constexpr bool _default_depth_mask = true;
	static constexpr std::tuple<CompareFunction, uint8_t, uint8_t> _default_stencil_func =
			{ CompareFunction::ALWAYS, 0x00, 0xFF };
	static constexpr uint8_t _default_stencil_mask = 0xFF;
	static constexpr std::tuple<StencilFunction, StencilFunction, StencilFunction> _default_stencil_op =
			{ StencilFunction::KEEP, StencilFunction::KEEP, StencilFunction::KEEP };

};

}

#endif
