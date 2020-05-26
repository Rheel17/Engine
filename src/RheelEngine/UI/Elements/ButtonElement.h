/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_BUTTONELEMENT_H
#define RHEELENGINE_BUTTONELEMENT_H
#include "../../_common.h"

#include "../Element.h"

namespace rheel {

class RE_API ButtonElement : public Element {

public:
	ButtonElement();
	ButtonElement(const std::string& text);
	ButtonElement(std::string&& text);

	template<typename Action>
	ButtonElement(Action&& action) :
			text("Button"),
			_action(std::forward<Action>(action)) {

		SetFocusable(true);
	}

	template<typename Action>
	ButtonElement(Action&& action, std::string text) :
			text(text),
			_action(action) {

		SetFocusable(true);
	}

	virtual ~ButtonElement() = default;

	ButtonElement(ButtonElement&&) = default;
	ButtonElement& operator=(ButtonElement&& button) = default;

	template<typename Action>
	void SetAction(Action&& action) {
		_action = std::forward<Action>(action);
	}

	virtual void PerformAction() const;

	virtual void OnMouseButtonPress(Input::MouseButton button, Input::Modifiers mods) override;
	virtual void OnMouseButtonRelease(Input::MouseButton button, Input::Modifiers mods) override;
	virtual void OnMouseEnter(const vec2& position) override;
	virtual void OnMouseExit(const vec2& position) override;

protected:
	virtual void DoDraw(float time, float dt) const override;

public:
	std::string text;
	std::reference_wrapper<const Font> font = Font::GetDefaultFont();

	Color color_foreground = 0x1d1d1e;
	Color color_foreground_rollover = 0x444851;
	Color color_foreground_active = 0x444851;
	Color color_foreground_disabled = 0x565a62;

	Color color_background = 0xdedfe2;
	Color color_background_rollover = 0xbcc6d1;
	Color color_background_active = 0x83a9af;
	Color color_background_disabled = 0x6f7072;

private:
	void DrawStandardButton_(Color foreground, Color background) const;

	std::function<void()> _action;

	bool _enabled = true;
	bool _rollover = false;
	bool _active = false;

};

}

#endif
