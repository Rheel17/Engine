/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "GameOverOverlay.h"

GameOverOverlay::GameOverOverlay(Wowie3& game) :
		_game(game) {}

void GameOverOverlay::Show(rheel::TextElement* score_element, float speed, float max_alpha, float wait_period) {
	_score_element = score_element;
	_fade_direction = speed;
	_max_alpha = max_alpha;
	_alpha = -wait_period / speed;
}

void GameOverOverlay::DoDraw(float time, float dt) const {
	_alpha += _fade_direction * dt;

	if (_score_element) {
		float text_alpha = 1.0f - (_alpha / _max_alpha);
		_score_element->color = { 0xECDCB0, static_cast<int>(text_alpha * 255) };
	}

	if (_alpha > _max_alpha) {
		_alpha = _max_alpha;
		_fade_direction = 0.0f;
	}

	if (_alpha <= 0.0f) {
		return;
	}

	DrawColoredQuad(GetBounds(), rheel::Color(0x2E2E3A, _alpha * 255));
}

void game_over_show::Update() {
	if (show.x > 0.0f) {
		element->Show(text_element, show.x, show.y, show.z);
		show.x = 0.0f;
	}
}
