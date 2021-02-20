#include <iostream>
#include <SDL2/SDL.h>
#include "controls.hpp"


void Controls::ResetMousePosition(SDL_Window* window, int center_x, int center_y)
{
    int mloc_x, mloc_y;
    SDL_GetRelativeMouseState(&mloc_x, &mloc_y);
    SDL_WarpMouseInWindow(window, center_x, center_y);
}

void Controls::Update(SDL_Event& sdlEvent, Camera* camera, const long deltaTime) {

    glm::fvec3& cPos = camera->transform.GetPosition();
    glm::fvec3& cRot = camera->transform.GetRotation();

    if (sdlEvent.type == SDL_MOUSEMOTION)
    {
        //handle mouse movements
        int x, y;
        SDL_GetRelativeMouseState(&x, &y);

        float yLimit = glm::radians(90.0f);
        float dx = x * mSensitivity * deltaTime;
        float dy = y * mSensitivity * deltaTime;

        cRot.x -= dx;
        cRot.y = glm::clamp(cRot.y - dy, -yLimit, yLimit);
    }

    //handle keyboard
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        cPos -= camera->transform.GetRight() * mSpeed * (float)deltaTime;
    }

    if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        cPos += camera->transform.GetRight() * mSpeed * (float)deltaTime;
    }

    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        cPos += camera->transform.GetDirection() * mSpeed * (float)deltaTime;
    }

    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        cPos -= camera->transform.GetDirection() * mSpeed * (float)deltaTime;
    }

    if (keystate[SDL_SCANCODE_SPACE]) {
        cPos += camera->transform.GetUp() * mSpeed * (float)deltaTime;
    }

    if (keystate[SDL_SCANCODE_LCTRL]) {
        cPos -= camera->transform.GetUp() * mSpeed * (float)deltaTime;
    }
}
