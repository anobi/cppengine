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
#include "Entity.hpp"

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

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), 45.0f, (float)800 / (float)600, 0.0f, 100.0f);

	std::vector<Entity*> entities;

	Entity* barrel = new Entity("../res/Barrel.blend", "default", "../res/Barrel.png");
	barrel->SetPosition(glm::vec3(-1.0, 0.0, 0.0));
	barrel->SetScale(glm::vec3(0.5f));
	entities.push_back(barrel);

	Entity* monkey = new Entity("../res/monkey3.obj", "default", "../res/Barrel.png");
	monkey->SetPosition(glm::vec3(1.0, 0.0, 0.0));
	monkey->SetScale(glm::vec3(0.5f));
	entities.push_back(monkey);

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

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update world

        //update entities
		int numEntities = entities.size();
		for (int i = 0; i < numEntities; i++) {
			entities[i]->Rotation()->y = counter * 10;
			entities[i]->Update(camera);
		}

		//render and refresh display
        _display.Update();
		counter += 0.001f;
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
