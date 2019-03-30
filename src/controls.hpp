#ifndef CONTROLS_H
#define CONTROLS_H

#include <memory>
#include <SDL2/SDL.h>
#include "camera.hpp"

class Controls {
public:
	Controls(){
		this->mSpeed = 0.02f;
		this->mSensitivity = 0.001f;
	};
	~Controls(){};

	void Update(SDL_Event &sdlEvent, std::shared_ptr<Camera> camera, const long deltaTime);

	inline void SetMovementSpeed(const float speed) { this->mSpeed = speed;}
	inline void SetSensitivity(const float sensitivity) { this->mSensitivity = sensitivity;}

private:
	float mSpeed;
	float mSensitivity;
};

#endif