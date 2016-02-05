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
#include "Mesh.hpp"
#include "Texture.hpp"

std::vector<SDL_Keycode> move_keys_down;

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
	if (!mDisplay.Init()) {
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";

	//Renderer
	std::cout << "* Remderer: ";
	if (!mRenderer.Init(&mDisplay)) {
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";
    
    //Input system
    std::cout << "* Input: ";
    if(!mInput.Init()){
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

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), 45.0f, (float)800 / (float)600, 0.1f, 100.0f);
	mRenderer.SetCamera(camera);

	std::vector<Entity*> entities;

	Entity barrel;
	barrel.GetTransform()->SetScale(glm::vec3(0.5f));
	barrel.GetTransform()->SetPosition(glm::vec3(-1.0f, -0.5f, 0.0f));
	barrel.AddComponent(new Shader("default"));
	barrel.AddComponent(new Texture("res/Barrel.png"));
	barrel.AddComponent(new Mesh("res/barrel.obj"));
	entities.push_back(&barrel);

	Entity box;
	box.GetTransform()->SetScale(glm::vec3(0.5f));
	box.GetTransform()->SetPosition(glm::vec3(1.0f, -0.5f, 0.0f));
	box.AddComponent(new Shader("default"));
	box.AddComponent(new Texture("res/Box.000.png"));
	box.AddComponent(new Mesh("res/uvcube.obj"));
	entities.push_back(&box);

	Entity monkey;
	monkey.GetTransform()->SetScale(glm::vec3(0.5f));
	monkey.GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	monkey.AddComponent(new Shader("default"));
	monkey.AddComponent(new Texture("res/Stone.Floor.001.png"));
	monkey.AddComponent(new Mesh("res/monkey3.obj"));
	entities.push_back(&monkey);

    SDL_Event event;
	float counter = 0.0f;
	float velX = 0.0f;
	float velY = 0.0f;
	float velZ = 0.0f;
	float speed = 0.02f;

    while(gameState == GAMESTATE_RUNNING) {
        auto loop_start = high_resolution_clock::now();
		auto loop_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
        auto delay = ((milliseconds)1000 / 60 - elapsed).count();

        if(delay >= 0){
            SDL_Delay(delay);
        }


		//
		// CONTROLS, WHOA
		//
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) {
			Quit();
		};

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					Quit();
					break;
				case SDLK_LEFT: velX++; break;
				case SDLK_RIGHT: velX--; break;
				case SDLK_UP: velZ++; break;
				case SDLK_DOWN: velZ--; break;
				case SDLK_LCTRL: velY--; break;
				case SDLK_SPACE: velY++; break;
			}
		}

		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_LEFT: velX = 0; break;
				case SDLK_RIGHT: velX = 0; break;
				case SDLK_UP: velZ = 0; break;
				case SDLK_DOWN: velZ = 0; break;
				case SDLK_LCTRL: velY = 0; break;
				case SDLK_SPACE: velY= 0; break;
			}
		}

		glm::vec3 cPos = camera.GetPosition();
		glm::vec3 movPos = glm::vec3(glm::clamp(velX, -speed, speed) * delay, glm::clamp(velY, -speed, speed) * delay, glm::clamp(velZ, -speed, speed) * delay);
		glm::vec3 movement = cPos + movPos;

		camera.SetPosition(movement);

		//end of controls, whoa

        //update world

        //update entities & render

		//TODO: figure out where to put this shit
		glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int numEntities = entities.size();
		for (int i = 0; i < numEntities; i++) {
			entities[i]->GetTransform()->GetRotation()->y = counter * 10;
			mRenderer.Render(*entities[i]);
		}

		mDisplay.Update();
		counter += 0.001f;
    }
    Shutdown();
}

void Game::Shutdown(){
    std::cout << "Shutting down...\n";

    mInput.Shutdown();
	mDisplay.Shutdown();

	SDL_Quit();
}

void Game::Quit(){
    std::cout << "QUIT\n";
    gameState = GAMESTATE_STOPPED;
}

std::vector<Entity>* Game::GetEntities(){
    return &entities;
};
