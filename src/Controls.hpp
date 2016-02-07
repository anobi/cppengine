#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include "lib/Camera.hpp"

class Controls {
public:
	Controls(){
		this->mSpeed = 0.02f;
		this->mSensitivity = 0.001f;
	};
	~Controls(){};

	void Update(SDL_Event* event, Camera* camera, const long deltaTime);

	inline void SetMovementSpeed(const float speed) { this->mSpeed = speed;}
	inline void SetSensitivity(const float sensitivity) { this->mSensitivity = sensitivity;}

private:
	float mSpeed;
	float mSensitivity;
};

#endif
