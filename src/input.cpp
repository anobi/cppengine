#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"
#include "input.hpp"

Game game;

Input::Input(){
    
}

bool Input::Init(){
    //init input devices etc
	SDL_Init(SDL_INIT_EVENTS);
    return true; 
}

void Input::Update(){

}
    
void Input::Shutdown(){
	std::cout << "* Input\n";
    //deinit shiet 
}
