#include <iostream>
#include <SDL.h>
#include "controls.hpp"


void Controls::ResetMousePosition(SDL_Window* window, int center_x, int center_y)
{
    int mloc_x, mloc_y;
    SDL_GetRelativeMouseState(&mloc_x, &mloc_y);
    SDL_WarpMouseInWindow(window, center_x, center_y);
}

void Controls::NewFrame(Camera* camera, const float delta_time) {
    this->_delta_time = delta_time;
    this->_position_accumulator = glm::zero<glm::fvec3>();
    this->_rotation_accumulator = glm::zero<glm::fvec3>();
    this->_camera = camera;
}

void Controls::Rotate(const int x, const int y) {
    float realMouseSensitivity = this->mouseSensitivity / 10000.0f;

    constexpr float yLimit = glm::radians(90.0f);
    float dx = x * realMouseSensitivity * this->_delta_time;
    float dy = y * realMouseSensitivity * this->_delta_time;

    this->_rotation_accumulator.x -= dx;
    this->_rotation_accumulator.y = glm::clamp(this->_rotation_accumulator.y - dy, -yLimit, yLimit);
}

void Controls::Move(const Uint8* keystate) 
{
    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        this->_position_accumulator -= this->_camera->transform.GetRight() * this->movementSpeed * this->_delta_time;
    }

    if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        this->_position_accumulator += this->_camera->transform.GetRight() * this->movementSpeed * this->_delta_time;
    }

    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        this->_position_accumulator += this->_camera->transform.GetDirection() * this->movementSpeed * this->_delta_time;
    }

    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        this->_position_accumulator -= this->_camera->transform.GetDirection() * this->movementSpeed * this->_delta_time;
    }

    if (keystate[SDL_SCANCODE_SPACE]) {
        this->_position_accumulator += this->_camera->transform.GetUp() * this->movementSpeed * this->_delta_time;
    }

    if (keystate[SDL_SCANCODE_LCTRL]) {
        this->_position_accumulator -= this->_camera->transform.GetUp() * this->movementSpeed * this->_delta_time;
    }
}

bool Controls::Commit() 
{
    bool updated = false;
    if (this->_position_accumulator.length() != 0.0f) {
        this->_camera->transform.GetPosition() += this->_position_accumulator;
        updated = true;
    }

    if (this->_position_accumulator.length() != 0.0f) {
        this->_camera->transform.GetRotation() += this->_rotation_accumulator;
        updated = true;
    }

    return updated;
}
