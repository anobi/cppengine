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
	if (!mDisplay.Init(1024, 700)) {
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

	SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mControls.SetSensitivity(0.00025f);
	
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

	Camera camera = Camera(new Transform(), glm::perspective(
							   45.0f, //FOV
							   mDisplay.GetAspectRatio(), //duh
							   0.1f, //depth aka znear
							   100.0f)); //zFar

	camera.mTransform->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	camera.mTransform->SetRotation(glm::vec3(0.0f, 0.0f, 1.0f));

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

	float counter = 0.0f;
	SDL_Event event;

    while(gameState == GAMESTATE_RUNNING) {
        auto loop_start = high_resolution_clock::now();
		auto loop_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
        auto delay = ((milliseconds)1000 / 60 - elapsed).count();

        if(delay >= 0){
            SDL_Delay(delay);
        }

		while(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) {
				Quit();
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						Quit();
						break;
				}
			}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_RESIZED:
						mDisplay.SetResolution(event.window.data1, event.window.data2, true);
						camera.SetAspectRatio(45.0f, mDisplay.GetAspectRatio());
						break;
				}
			}
		}

		//skip controls if we don't have focus
		mControls.Update(&event, &camera, delay);
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
