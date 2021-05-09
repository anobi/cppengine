#include <iostream>
#include <SDL2/SDL.h>
#include "controls.hpp"


void Controls::ResetMousePosition(SDL_Window* window, int center_x, int center_y)
{
    int mloc_x, mloc_y;
    SDL_GetRelativeMouseState(&mloc_x, &mloc_y);
    SDL_WarpMouseInWindow(window, center_x, center_y);
}

bool Controls::Update(SDL_Event& sdlEvent, Camera* camera, const float deltaTime) {

    glm::fvec3 oPos = camera->transform.GetPosition();
    glm::fvec3 oRot = camera->transform.GetRotation();
    glm::fvec3& cPos = camera->transform.GetPosition();
    glm::fvec3& cRot = camera->transform.GetRotation();

    if (sdlEvent.type == SDL_MOUSEMOTION)
    {
        //handle mouse movements
        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        float realMouseSensitivity = this->mouseSensitivity / 10000.0f;

        float yLimit = glm::radians(90.0f);
        float dx = x * realMouseSensitivity * deltaTime;
        float dy = y * realMouseSensitivity * deltaTime;

        cRot.x -= dx;
        cRot.y = glm::clamp(cRot.y - dy, -yLimit, yLimit);
    }

    //handle keyboard
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        cPos -= camera->transform.GetRight() * this->movementSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        cPos += camera->transform.GetRight() * this->movementSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        cPos += camera->transform.GetDirection() * this->movementSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        cPos -= camera->transform.GetDirection() * this->movementSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_SPACE]) {
        cPos += camera->transform.GetUp() * this->movementSpeed * deltaTime;
    }

    if (keystate[SDL_SCANCODE_LCTRL]) {
        cPos -= camera->transform.GetUp() * this->movementSpeed * deltaTime;
    }

    if (cPos != oPos || cRot != oRot) {
        return true;
    }

    return false;
}
