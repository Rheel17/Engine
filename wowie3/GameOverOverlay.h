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

	void Show(float speed);

protected:
	void DoDraw(float time, float dt) const override;

private:
	Wowie3& _game;
	mutable float _alpha = 0.0f;
	mutable float _fade_direction = 0.0f;

};

struct game_over_show : public rheel::Component {
	static constexpr rheel::ComponentId id = 3;
	GameOverOverlay* element;
	float show = 0.0f;

	void Update() override;
};

#endif
