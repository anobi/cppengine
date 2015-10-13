#include <iostream>
#include <SDL2/SDL.h>
#include "g_game.hpp"
#include "g_input.hpp"

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
