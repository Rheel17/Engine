/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_MAINMENU_H
#define ENGINE_MAINMENU_H

#include "Wowie3.h"

class MainMenu : public rheel::Element {

public:
	explicit MainMenu(Wowie3& game);

	void OnKeyPress(rheel::Input::Key key, rheel::Input::Scancode scancode, rheel::Input::Modifiers mods) override;

protected:
	void DoDraw(float time, float dt) const override;

private:
	Wowie3& _game;

};

#endif
