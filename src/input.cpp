#include <cstdio>
#include <SDL.h>
#include "game.hpp"
#include "input.hpp"

bool Input::Init() {
    //init input devices etc
    SDL_Init(SDL_INIT_EVENTS);
    return true;
}

void Input::Update() {

}

void Input::Shutdown() {
    printf("* Input\n");
    //deinit shiet 
}
