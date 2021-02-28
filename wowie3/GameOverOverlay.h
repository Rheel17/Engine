/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_GAMEOVEROVERLAY_H
#define ENGINE_GAMEOVEROVERLAY_H

#include <RheelEngine.h>

#include "Wowie3.h"

class GameOverOverlay : public rheel::Element {

public:
	explicit GameOverOverlay(Wowie3& game);

	void Show(rheel::TextElement* score_element, float speed, float max_alpha = 1.0f, float wait_period = 0.0f);
	void OnKeyPress(rheel::Input::Key key, rheel::Input::Scancode scancode, rheel::Input::Modifiers mods) override;

protected:
	void DoDraw(float time, float dt) const override;

private:
	Wowie3& _game;
	rheel::TextElement* _score_element{};
	float _max_alpha = 1.0f;
	mutable float _alpha = 0.0f;
	mutable float _fade_direction = 0.0f;

};

struct game_over_show : public rheel::Component {
	static constexpr rheel::ComponentId id = 3;
	GameOverOverlay* element;
	rheel::TextElement* text_element;
	vec3 show{0.0f, 1.0f, 0.0f};

	void Update() override;
};

#endif
