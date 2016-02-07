#include <iostream>
#include <SDL2/SDL.h>
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
