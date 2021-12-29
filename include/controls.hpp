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

    void ResetMousePosition(SDL_Window* window, int center_x, int center_y);

    void Rotate(const int x, const int y);
    void Move(const Uint8* keystate);

    void NewFrame(Camera* camera, const float delta_time);
    bool Commit();

    float movementSpeed;
    float mouseSensitivity;

private:
    Camera* _camera;
    float _delta_time;
    glm::fvec3 _position_accumulator;
    glm::fvec3 _rotation_accumulator;
};

#endif
