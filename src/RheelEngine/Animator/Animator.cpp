/*
 * Copyright (c) 2020 Levi van Rheenen
 */
#include "Animator.h"

namespace rheel {

void Animator::AddClip(const std::string& name, Clip clip) {
	PlayState state = _play_state;

	if (_play_state != PlayState::STOPPED) {
		RemoveClip(name);
	}

	_clips[name] = std::make_unique<Clip>(std::move(clip));

	switch (state) {
		case PlayState::STOPPED:
			break;
		case PlayState::PLAYING:
			Play(name);
			break;
		case PlayState::LOOPING:
			Loop(name);
			break;
	}
}

void Animator::RemoveClip(const std::string& name) {
	if (name == _current_clip) {
		Stop();
	}

	_clips.erase(name);
}

Clip& Animator::GetClip(const std::string& name) {
	return *_clips.find(name)->second;
}

const Clip& Animator::GetClip(const std::string& name) const {
	return *_clips.find(name)->second;
}

void Animator::Play(const std::string& name) {
	if (_play_state != PlayState::STOPPED) {
		Stop();
	}

	_current_clip = name;
	_play_state = PlayState::PLAYING;
	_time = 0.0f;
}

void Animator::Loop(const std::string& name) {
	if (_play_state != PlayState::STOPPED) {
		Stop();
	}

	_current_clip = name;
	_play_state = PlayState::LOOPING;
	_time = 0.0f;
}

void Animator::Stop() {
	_current_clip.clear();
	_play_state = PlayState::STOPPED;
}

const std::string& Animator::GetCurrentClip() {
	return _current_clip;
}

Animator::PlayState Animator::GetCurrentPlayState() {
	return _play_state;
}

void Animator::Update(float dt) {
	if (_play_state == PlayState::STOPPED) {
		return;
	}

	Clip& clip = GetClip(_current_clip);
	clip.Update(_time);

	if (_time >= clip.GetMaxTime()) {
		if (_play_state == PlayState::LOOPING) {
			_time = 0.0f;
		} else {
			Stop();
		}
	} else {
		_time += dt;
	}
}

}