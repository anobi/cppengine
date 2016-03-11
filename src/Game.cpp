#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

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
	if (!mDisplay.Init(1440, 900)) {
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";

	//Renderer
	std::cout << "* Renderer: ";
	if (!mRenderer.Init()) {
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

	//Init everything
    if(Init()){
		//Build the scene, a temp solution
		ConstructScene();

		//Start the game loop
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

	float counter = 0.0f;
	SDL_Event event;

    while(gameState == GAMESTATE_RUNNING) {
		
		/*************************
		* Update clock and so on *
		**************************/
        auto loop_start = high_resolution_clock::now();
		auto loop_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
        auto delay = ((milliseconds)1000 / 60 - elapsed).count();

        if(delay >= 0){
            SDL_Delay(delay);
        }

		/*****************************
		* Handle controls and events *
		******************************/

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
						mRenderer.GetCamera()->SetAspectRatio(45.0f, mDisplay.GetAspectRatio());
						break;
				}
			}
		}

		mControls.Update(event, mRenderer.GetCamera(), delay);

		/*****************************
		* Update entities and render *
		******************************/
		GetEntity("LightO")->GetTransform().SetPosition(glm::fvec3(glm::sin(counter * 50) * 5, 3.0f, glm::cos(counter * 50) * 5));

		//TODO: figure out where to put this shit
		glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int numEntities = entities.size();
		for (int i = 0; i < numEntities; i++) {
			mRenderer.Render(entities[i]);
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

std::vector<EntityRef> Game::GetEntities(){
    return entities;
};

EntityRef Game::AddEntity(EntityRef entity) {
	this->entities.push_back(entity);
	return entities.back();
}

EntityRef Game::GetEntity(const std::string name) {
	EntityRef entity = NULL;
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (entities[i]->GetName() == name) {
			entity = entities[i];
			break;
		}
	}
	return entity;
}

void Game::ConstructScene() {

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(Camera(glm::perspective(
		45.0f, //FOV
		mDisplay.GetAspectRatio(), //duh
		0.1f, //depth aka znear
		100.0f))); //zFar

	camera->mTransform.SetPosition(glm::fvec3(0.0f, 5.0f, 12.0f));
	camera->mTransform.SetRotation(glm::fvec3(glm::radians(180.0f), glm::radians(-20.0f), 0.0f));
	mRenderer.SetCamera(camera);

	/*
	Meshes
	*/

	//Room
	EntityRef room = AddEntity(std::make_shared<Entity>(Entity("Room")));
	std::shared_ptr<Material> roomMat = std::make_shared<Material>();

	room->GetTransform().SetScale(glm::fvec3(1.0f));
	room->GetTransform().SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f));
	roomMat->SetAlbedoMap("res/Rock.Wall.000.png");
	roomMat->SetNormalMap("res/Rock.Wall.Normal.png");
	roomMat->SetAlbedo(glm::fvec3(1.0f));
	room->AddComponent(roomMat);
	room->AddComponent(std::make_shared<Shader>("default"));
	room->AddComponent(std::make_shared<Mesh>("res/room.obj"));

	//Barrel
	EntityRef barrel = AddEntity(std::make_shared<Entity>(Entity("Barrel")));
	std::shared_ptr<Material> barrelMat = std::make_shared<Material>();

	barrel->GetTransform().SetScale(glm::fvec3(1.0f));
	barrel->GetTransform().SetPosition(glm::fvec3(-3.0f, 1.5f, 0.0f));
	barrel->GetTransform().SetRotation(glm::fvec3(glm::radians(90.0f), 0.0f, 0.0f));
	barrelMat->SetAlbedoMap("res/Barrel.png");
	barrelMat->SetNormalMap("res/Barrel.Normal.png");
	barrelMat->SetAlbedo(glm::fvec3(1.0f));
	barrel->AddComponent(barrelMat);
	barrel->AddComponent(std::make_shared<Shader>("default"));
	barrel->AddComponent(std::make_shared<Mesh>("res/barrel.obj"));

	//Box
	EntityRef box = AddEntity(std::make_shared<Entity>("Box"));
	std::shared_ptr<Material> boxMat = std::make_shared<Material>();

	box->GetTransform().SetScale(glm::fvec3(1.0f));
	box->GetTransform().SetPosition(glm::fvec3(3.0f, 1.0f, -2.0f));
	box->GetTransform().SetRotation(glm::fvec3(0.0f, glm::radians(30.0f), 0.0f));
	boxMat->SetAlbedoMap("res/Box.000.png");
	boxMat->SetNormalMap("res/Box.Normal.png");
	boxMat->SetAlbedo(glm::fvec3(1.0f));
	box->AddComponent(boxMat);
	box->AddComponent(std::make_shared<Shader>("default"));
	box->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));

	//Suzanne
	EntityRef monkey = AddEntity(std::make_shared<Entity>("Monkey"));
	std::shared_ptr<Material> monkeyMat = std::make_shared<Material>();

	monkey->GetTransform().SetScale(glm::fvec3(1.0f));
	monkey->GetTransform().SetPosition(glm::fvec3(0.0f, 0.4f, 2.0f));
	monkey->GetTransform().SetRotation(glm::fvec3(glm::radians(-38.0f), 0.0f, 0.0f));
	monkeyMat->SetAlbedoMap("res/Stone.Floor.001.png");
	//monkeyMat->SetNormalMap("res/Stone.Floor.Normal.png");
	monkeyMat->SetAlbedo(glm::fvec3(1.0f));
	monkey->AddComponent(monkeyMat);
	monkey->AddComponent(std::make_shared<Shader>("default"));
	monkey->AddComponent(std::make_shared<Mesh>("res/monkey3.obj"));


	/*
	Lights
	*/

	//warm foreground light
	EntityRef light = AddEntity(std::make_shared<Entity>("LightY"));
	light->GetTransform().SetPosition(glm::fvec3(-8.0f, 8.0f, 8.0f));
	light->GetTransform().SetScale(glm::fvec3(0.2f));
	light->AddComponent(std::make_shared<PointLight>(glm::fvec3(1.0f, 0.9f, 0.8f), 1.0f, 0.5f, 10.0f));
	light->AddComponent(std::make_shared<Shader>("default"));
	light->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));

	//TODO: need to figure out how to automate adding lights to renderer
	mRenderer.AddLight(light->GetComponent("PointLight"));

	//cool background light
	EntityRef light2 = AddEntity(std::make_shared<Entity>("LightB"));
	light2->GetTransform().SetPosition(glm::fvec3(8.0f, 8.0f, -8.0f));
	light2->GetTransform().SetScale(glm::fvec3(0.2f));
	light2->AddComponent(std::make_shared<PointLight>(glm::fvec3(0.5f, 0.75f, 1.0f), 1.0f, 0.5f, 10.0));
	light2->AddComponent(std::make_shared<Shader>("default"));
	light2->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));
	mRenderer.AddLight(light2->GetComponent("PointLight"));

	//awesome spinning FIRE BALL LIGHT YEAH
	EntityRef light3 = AddEntity(std::make_shared<Entity>("LightO"));
	light3->GetTransform().SetPosition(glm::fvec3(0.0f, 3.0f, 0.0f));
	light3->GetTransform().SetScale(glm::fvec3(0.2f));
	light3->AddComponent(std::make_shared<PointLight>(glm::fvec3(1.0f, 0.4f, 0.0f), 1.0f, 0.0f, 4.0f));
	light3->AddComponent(std::make_shared<Shader>("default"));
	light3->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));
	mRenderer.AddLight(light3->GetComponent("PointLight"));

}