/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#include "MainMenu.h"

MainMenu::MainMenu(Wowie3& game) :
		_game(game) {
	SetFocusable(true);
}

void MainMenu::DoDraw(float time, float dt) const {
	auto& tr = GetTextRenderer();
	auto bounds = GetBounds();
	tr.DrawParagraph("Memomaze", 0, bounds.height * 0.2f, bounds.width, _game.font, bounds.height * 0.05f, 0xECDCB0, TextAlign::CENTER);

	tr.DrawParagraph("Press <SPACE> to start", 0, bounds.height * 0.5f, bounds.width, _game.font, bounds.height * 0.02f, 0xECDCB0, TextAlign::CENTER);
}

void MainMenu::OnKeyPress(rheel::Input::Key key, rheel::Input::Scancode scancode, rheel::Input::Modifiers mods) {
	if (key == rheel::Input::Key::KEY_SPACE) {
		_game.RunAfterCurrentFrame([this] { _game.NewGame(); });
	}
}
