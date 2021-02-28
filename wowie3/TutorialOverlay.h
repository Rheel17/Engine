/*
 * Copyright (c) 2021 Levi van Rheenen
 */
#ifndef ENGINE_TUTORIALOVERLAY_H
#define ENGINE_TUTORIALOVERLAY_H

#include <RheelEngine.h>

class TutorialOverlay : public rheel::Element {

public:
	explicit TutorialOverlay(rheel::Game& game);

	void Hide();
	void Show();

protected:
	void DoDraw(float time, float dt) const override;

private:
	void _draw(const rheel::Image& image, float x, float y) const;

	rheel::Image _up;
	rheel::Image _down;
	rheel::Image _left;
	rheel::Image _right;
	mutable float _alpha_direction = 0.0f;
	mutable float _alpha = 1.0f;
};

struct tutorial_hide : public rheel::Component {
	static constexpr rheel::ComponentId id = 2;
	TutorialOverlay* element;
	bool hide = false;

	void Update() override;
};

#endif
