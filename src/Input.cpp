#include <iostream>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Game.hpp"
#include "Input.hpp"

Game game;

Input::Input(){
    
}

bool Input::Init(){
    //init input devices etc
    return true; 
}

void Input::Update(){

}
    
void Input::Shutdown(){
	std::cout << "* Input\n";
    //deinit shiet 
}
