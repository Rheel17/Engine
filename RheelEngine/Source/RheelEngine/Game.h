#ifndef GAME_H_
#define GAME_H_

namespace rheel {

class Game {

public:
	virtual ~Game() = default;

	virtual void RegisterComponents() {}
	virtual void RegisterScripts() {}
	virtual void RegisterBlueprints() {}
	virtual void RegisterSceneDescriptions() {}

	/**
	 * Initializes the game. This is before a window with OpenGL context is
	 * created. Any render initialization cannot be done in this method, use
	 * Game::Start() for that.
	 */
	virtual void Initialize() {}

	/**
	 * Called when the game is started. At this point, a window and OpenGL context
	 * are present. This is the final method called before the main game loop.
	 */
	virtual void Start() {}

};

}

#endif
