/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "TutorialOverlay.h"

TutorialOverlay::TutorialOverlay(rheel::Game& game) :
		_up(game.GetAssetLoader().png.Load("button_up.png")),
		_down(game.GetAssetLoader().png.Load("button_down.png")),
		_left(game.GetAssetLoader().png.Load("button_left.png")),
		_right(game.GetAssetLoader().png.Load("button_right.png")) {}

void TutorialOverlay::Hide() {
	_alpha_direction = -2.5f;
}

void TutorialOverlay::Show() {
	_alpha_direction = 0.0f;
	_alpha = 1.0f;
}

void TutorialOverlay::DoDraw(float time, float dt) const {
	_alpha += _alpha_direction * dt;

	if (_alpha <= 0.0f) {
		_alpha = 0.0f;
		_alpha_direction = 0.0f;
	}

	_draw(_up, 0.5f, 0.2f);
	_draw(_down, 0.5f, 0.8f);
	_draw(_left, 0.3f, 0.5f);
	_draw(_right, 0.7f, 0.5f);

	auto bounds = GetBounds();
	GetTextRenderer().DrawParagraph("Explore the entire maze without finding the exit.\nHitting a wall ends the game.", 0, bounds.height * 0.62f, bounds.width,
			rheel::Font::GetFont("font"), bounds.height * 0.03f, { 0xECDCB0, (int) (_alpha * 255) }, TextAlign::CENTER);
}

void TutorialOverlay::_draw(const rheel::Image& image, float xf, float yf) const {
	constexpr float width_f = 0.06f;
	constexpr float height_f = 0.4f * width_f;

	auto bounds = GetBounds();

	unsigned x = xf * bounds.width;
	unsigned y = yf * bounds.height;
	unsigned width = bounds.width * width_f;
	unsigned height = bounds.width * height_f;

	DrawTexturedQuad({ x - width / 2, y - height / 2, width, height }, image, _alpha);
}

void tutorial_hide::Update() {
	if (hide && element) {
		element->Hide();
		hide = false;
	}
}
