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

	std::string heading;
	std::string explanation;
	std::string score_text;

	if (_game.HasVisitedAll()) {
		// We got all tiles
		heading = "Winner!";
		explanation = "You explored the entire maze";
		score_text = "Your final score is";
	} else if (_max_alpha == 1.0f) {
		// We exited the maze
		heading = "Oh no!";
		explanation = "You exited the maze.";
		score_text = "Your score would have been";
	} else {
		// We hit a wall
		heading = "The end";
		explanation = "You hit a wall and ended the game.";
		score_text = "Your final score is";
	}

	score_text += ": " + _score_element->text;

	auto& tr = GetTextRenderer();
	auto w = GetBounds().width;
	auto h = GetBounds().height;
	int alpha = (std::pow(_alpha, 4) / std::pow(_max_alpha, 4)) * 255;
	tr.DrawParagraph(heading, 0, h * 0.2f, w, _game.font, h * 0.05f, { 0xECDCB0, alpha }, TextAlign::CENTER);
	tr.DrawParagraph(explanation, 0, h * 0.3f, w, _game.font, h * 0.02f, { 0xECDCB0, alpha }, TextAlign::CENTER);
	tr.DrawParagraph(score_text, 0, h * 0.5f, w, _game.font, h * 0.034f, { 0xECDCB0, alpha }, TextAlign::CENTER);
	tr.DrawParagraph("<ESC> Exit the game      <SPACE> New game      <M> Main menu",
			0, h * 0.8f, w, _game.font, h * 0.018f, { 0xECDCB0, alpha }, TextAlign::CENTER);
}

void game_over_show::Update() {
	if (show.x > 0.0f) {
		element->Show(text_element, show.x, show.y, show.z);
		show.x = 0.0f;
	}
}
