/*
 * Copyright (c) Levi van Rheenen. All rights reserved.
 */
#ifndef GAME_H_
#define GAME_H_
#include "_common.h"

namespace rheel {

class RE_API Game {

public:
	virtual ~Game() = default;

	/**
	 * Initializes the game. This is before a window with _OpenGL context is
	 * created. Any render initialization cannot be done in this method, use
	 * Game::Start() for that.
	 */
	virtual void Initialize() {}

	/**
	 * Called when the game is started. At this point, a window and _OpenGL
	 * context are present. This is the final method called before the main game
	 * loop.
	 */
	virtual void Start() {}

};

}

#endif
