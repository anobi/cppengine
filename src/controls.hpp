#ifndef CONTROLS_H
#define CONTROLS_H

#include <memory>
#include <SDL.h>
#include <glm/glm.hpp>
#include "camera.hpp"

class Controls {
public:
    Controls() {
        this->movementSpeed = 0.02f;
        this->mouseSensitivity = 3.0f;
    };
    ~Controls() {};

    bool Update(SDL_Event& sdlEvent, Camera* camera, const float deltaTime);
    void ResetMousePosition(SDL_Window* window, int center_x, int center_y);

    float movementSpeed;
    float mouseSensitivity;
};

#endif
