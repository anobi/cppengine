#include <iostream>
#include <chrono>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

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
	if (!_display.Init()) {
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";
    
    //Input system
    std::cout << "* Input: ";
    if(!_input.Init()){
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

	Mesh cube = Mesh("../res/monkey3.obj");
	Shader shader = Shader("default");
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, -10.0f), 45.0f, (float)800 / (float)600, 0.1f, 100.0f);
	Transform transform;

    SDL_Event event;
	float counter = 0.0f;
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
		for(auto e : entities){
			e.Update();
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		transform.Rotation()->y = counter * 100;

        //update world
		shader.Bind();
		shader.Update(transform, camera);
		cube.Draw();

		//render and refresh display
        _display.Update();
		counter += 0.0001f;
    }
    Shutdown();
}

void Game::Shutdown(){
    std::cout << "Shutting down...\n";

    _input.Shutdown();
	_display.Shutdown();

	SDL_Quit();
}

void Game::Quit(){
    std::cout << "QUIT\n";
    gameState = GAMESTATE_STOPPED;
}

std::vector<Entity>* Game::GetEntities(){
    return &entities;
};
