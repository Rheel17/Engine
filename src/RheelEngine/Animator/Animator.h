/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#ifndef RHEELENGINE_ANIMATOR_H
#define RHEELENGINE_ANIMATOR_H
#include "../_common.h"

#include "Clip.h"

namespace rheel {

class RE_API Animator {

public:
	enum class PlayState {
		STOPPED, PLAYING, LOOPING
	};

public:
	/**
	 * Adds a clip to the animator. The name should be used to reference the
	 * clip in the future. If a clip with that name already exists, it will
	 * first be removed. If that clip is playing, it will be stopped and this
	 * clip will be either played or looped, depending on the state of the
	 * removed clip.
	 *
	 * Note: the empty string ("") is not a valid clip name. When it is given,
	 * this function will throw an error.
	 */
	void AddClip(const std::string& name, Clip clip);

	/**
	 * Removes the clip from the animator. If the clip is currently playing, it
	 * will first be stopped.
	 */
	void RemoveClip(const std::string& name);

	/**
	 * Returns the clip object with the given name. If the name does not have a
	 * clip registered to it, this will fail.
	 */
	Clip& GetClip(const std::string& name);

	/**
	 * Returns the clip object with the given name. If the name does not have a
	 * clip registered to it, this will fail.
	 */
	const Clip& GetClip(const std::string& name) const;

	/**
	 * Plays an animation clip once. Any other previous animation will be
	 * stopped. If the given name is the name of the currently playing clip, it
	 * will be restarted.
	 */
	void Play(const std::string& name);

	/**
	 * Loops an animation clip. Any other previous animation will be
	 * stopped. If the given name is the name of the currently playing clip, it
	 * will be restarted.
	 */
	void Loop(const std::string& name);

	/**
	 * Stops the current animation clip
	 */
	void Stop();

	/**
	 * Returns the name of the current clip. This will return the empty string
	 * if no clip is currently playing.
	 */
	const std::string& GetCurrentClip();

	/**
	 * Returns the current playing state.
	 */
	PlayState GetCurrentPlayState();

	/**
	 * Updates this animator; animates the next frame
	 */
	void Update(float dt);

private:
	std::unordered_map<std::string, std::unique_ptr<Clip>> _clips;

	std::string _current_clip;
	PlayState _play_state;
	float _time;

};

}

#endif
