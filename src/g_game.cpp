#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include "g_game.hpp"
#include "d_display.hpp"
#include "g_input.hpp"

Display display;
Input input;

Game::Game(){
    gameState = GAMESTATE_STOPPED; 
}

bool Game::Init(){
    std::cout << "Initializing game...\n" ;

    //init stuff
    std::cout << "* SDL: ";
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cout << "Error: %s\n", SDL_GetError();
		return false;
    } else std::cout << "done\n";

    //Display
    std::cout << "* Display: ";
    if(!display.Init()){
        std::cout << "Error: %s\n", SDL_GetError();
		return false;
    } else std::cout << "done\n";
    
	//Input system
    std::cout << "* Input: ";
    if(!input.Init()){
        std::cout << "Error: %s\n", SDL_GetError();
		return false;
    } else std::cout << "done\n";

	return true;
}

void Game::Start(){
    if(Init()){
        gameState = GAMESTATE_RUNNING;
        Loop();
    } else {
        std::cout << "Failed to initialize game \n";
        exit(EXIT_FAILURE);
    }
}

void Game::Loop(){
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;

	SDL_Event event;
    while(gameState == GAMESTATE_RUNNING) {

		auto loop_start = high_resolution_clock::now();

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				Quit();
			};
			if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					Quit();
					break;
				}
			}
		}
		auto loop_end = high_resolution_clock::now();
		auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
		auto delay = ((milliseconds)1000 / 60 - elapsed).count();

		if(delay >= 0){
			SDL_Delay(delay);
		}
        //update entities
        //update world
        //render
		display.Update();
    }
    Shutdown();
}

void Game::Shutdown(){
	std::cout << "Shutting down...\n";
	input.Shutdown();
	display.Shutdown();
	SDL_Quit();
}

void Game::Quit(){
	std::cout << "QUIT\n";
	gameState = GAMESTATE_STOPPED;
}
