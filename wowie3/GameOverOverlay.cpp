/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "GameOverOverlay.h"

GameOverOverlay::GameOverOverlay(Wowie3& game) :
		_game(game) {}

void GameOverOverlay::Show(float speed) {
	_fade_direction = speed;
}

void GameOverOverlay::DoDraw(float time, float dt) const {
	if (_fade_direction <= 0.0f && _alpha <= 0.0f) {
		return;
	}

	_alpha += _fade_direction * dt;

	if (_alpha > 1.0f) {
		_alpha = 1.0f;
		_fade_direction = 0.0f;
	}

	DrawColoredQuad(GetBounds(), rheel::Color(0x2E2E3A, _alpha * 255));
}

void game_over_show::Update() {
	if (show > 0.0f) {
		element->Show(show);
		show = 0.0f;
	}
}
