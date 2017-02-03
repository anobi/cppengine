#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

#include "lib/imgui.h"
#include "lib/imgui_internal.h"
#include "lib/imgui_impl_sdl_gl3.h"

#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

Game::Game()
{
    gameState = GAMESTATE_STOPPED; 
}

int selected_entity = 0;

bool Game::Init()
{
    std::cout << "Initializing game...\n" ;

    //init stuff
    std::cout << "* SDL: ";
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
        std::cout << "Error: %s\n", SDL_GetError();
        return false;
    } 
	else std::cout << "done\n";

	//Display
	std::cout << "* Display: ";
	if (!mDisplay.Init(1440, 900)) 
	{
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";

	//Renderer
	std::cout << "* Renderer: ";
	if (!mRenderer.Init()) 
	{
		std::cout << "Error: %s\n", SDL_GetError();
		return false;
	}
	else std::cout << "done\n";
    
    //Input system
    std::cout << "* Input: ";
    if(!mInput.Init())
	{
        std::cout << "Error: %s\n", SDL_GetError();
        return false;
    } 
	else std::cout << "done\n";

	ImGui_ImplSdlGL3_Init(mDisplay.GetWindow());

	SDL_SetRelativeMouseMode(SDL_TRUE);

	mControls.SetSensitivity(0.00025f);
	
    return true;
}

void Game::Start()
{

	//Init everything
    if(Init())
	{
		//Build the scene, a temp solution
		ConstructScene();

		//Start the game loop
        gameState = GAMESTATE_RUNNING;
        Loop();
    } 
	else 
	{
        std::cout << "Failed to initialize game \n";
        exit(EXIT_FAILURE);
    }
}

void Game::Loop()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

	float counter = 0.0f;
	SDL_Event event;

    while(gameState == GAMESTATE_RUNNING) 
	{
		ImGui_ImplSdlGL3_NewFrame(mDisplay.GetWindow());
		
		/*************************
		* Update clock and so on *
		**************************/
        auto loop_start = high_resolution_clock::now();
		auto loop_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
        auto delay = ((milliseconds)1000 / 60 - elapsed).count();

        if(delay >= 0) SDL_Delay(delay);

		/*****************************
		* Handle controls and events *
		******************************/

		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) 
			{
				Quit();
			}

			if (event.type == SDL_KEYDOWN) 
			{
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						Quit();
						break;

					case SDLK_F1:
						menu = !menu;
						SDL_ShowCursor(menu);
						SDL_SetRelativeMouseMode((SDL_bool)!menu);
						break;
				}
			}

			if(event.type == SDL_WINDOWEVENT)
			{
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						mDisplay.SetResolution(event.window.data1, event.window.data2, true);
						mRenderer.GetCamera()->SetAspectRatio(45.0f, mDisplay.GetAspectRatio());
						break;
				}
			}
		}

		if (!menu) {
			mControls.Update(event, mRenderer.GetCamera(), delay);
		}

		/*****************************
		* Update entities and render *
		******************************/
		GetEntity("LightO")->GetTransform().SetPosition(glm::fvec3(glm::sin(counter * 50) * 5, 3.0f, glm::cos(counter * 50) * 5));

		//TODO: figure out where to put this shit
		glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int numEntities = entities.size();
		for (int i = 0; i < numEntities; i++) 
		{
			mRenderer.Render(entities[i]);
		}

		UpdateUI();
		ImGui::Render();

		mDisplay.Update();
		//SDL_GL_SwapWindow(mDisplay.GetWindow());

		counter += 0.001f;
    }

    Shutdown();
}

void Game::Shutdown() 
{
    std::cout << "Shutting down...\n";

	ImGui::Shutdown();
    mInput.Shutdown();
	mDisplay.Shutdown();

	SDL_Quit();
}

void Game::Quit()
{
    std::cout << "QUIT\n";
    gameState = GAMESTATE_STOPPED;
}

std::vector<EntityRef> Game::GetEntities()
{
    return entities;
};

EntityRef Game::AddEntity(EntityRef entity) 
{
	this->entities.push_back(entity);
	return entities.back();
}

EntityRef Game::GetEntity(const std::string name) 
{
	EntityRef entity = NULL;
	for (unsigned int i = 0; i < entities.size(); i++) 
	{
		if (entities[i]->GetName() == name) 
		{
			entity = entities[i];
			break;
		}
	}
	return entity;
}

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

void Game::UpdateUI()
{
	glm::fvec3 cPos = mRenderer.GetCamera()->GetPosition();

	std::vector<std::string> entity_list;	
	
	for (int i = 0; i < entities.size(); i++) 
	{
		EntityRef e = entities[i];
		glm::fvec3 pos = e->GetTransform().GetPosition();

		char buf[256];
		sprintf_s(buf, 256, "%s", e->GetName().c_str(), pos.x, pos.y, pos.z);
		entity_list.push_back(buf);
	}

	// Debug info window
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 80), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Info");
	ImGui::Text("Player position x: %.2f y: %.2f z: %.2f", cPos.x, cPos.y, cPos.z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// Entity debug window
	ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Entities");

	ImGui::PushItemWidth(-1);
	ImGui::ListBox("##entities", &selected_entity, vector_getter, static_cast<void*>(&entity_list), entity_list.size());
	ImGui::PopItemWidth();

	glm::fvec3 e_pos = entities[selected_entity]->GetTransform().GetPosition();
	ImGui::Text("Location x: %.2f y: %.2f z:%.2f", e_pos.x, e_pos.y, e_pos.z);

	ImGui::SliderFloat("X", &e_pos.x, -100, 100);
	ImGui::SliderFloat("Y", &e_pos.y, -100, 100);
	ImGui::SliderFloat("Z", &e_pos.z, -100, 100);

	entities[selected_entity]->GetTransform().SetPosition(e_pos);

	ImGui::End();
}

void Game::ConstructScene() 
{

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
	roomMat->SetAlbedoMap("res/bricks2.jpg");
	roomMat->SetNormalMap("res/bricks2_normal.jpg");
	roomMat->SetHeightMap("res/bricks2_disp.jpg");
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
	barrelMat->SetHeightMap("res/Barrel.Disp.png");
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

	//															color			   int    cut  radius
	light->AddComponent(std::make_shared<PointLight>(glm::fvec3(1.0f, 0.9f, 0.8f), 1.0f, 0.2f, 10.0f));
	light->AddComponent(std::make_shared<Shader>("default"));
	light->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));

	//TODO: need to figure out how to automate adding lights to renderer
	mRenderer.AddLight(light->GetComponent("PointLight"));

	//cool background light
	EntityRef light2 = AddEntity(std::make_shared<Entity>("LightB"));
	light2->GetTransform().SetPosition(glm::fvec3(8.0f, 8.0f, -8.0f));
	light2->GetTransform().SetScale(glm::fvec3(0.2f));
	light2->AddComponent(std::make_shared<PointLight>(glm::fvec3(0.5f, 0.75f, 1.0f), 1.0f, 0.2f, 10.0));
	light2->AddComponent(std::make_shared<Shader>("default"));
	light2->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));
	mRenderer.AddLight(light2->GetComponent("PointLight"));

	//awesome spinning FIRE BALL LIGHT YEAH
	EntityRef light3 = AddEntity(std::make_shared<Entity>("LightO"));
	light3->GetTransform().SetPosition(glm::fvec3(0.0f, 3.0f, 0.0f));
	light3->GetTransform().SetScale(glm::fvec3(0.2f));
	light3->AddComponent(std::make_shared<PointLight>(glm::fvec3(1.0f, 0.4f, 0.0f), 1.0f, 0.1f, 4.0f));
	light3->AddComponent(std::make_shared<Shader>("default"));
	light3->AddComponent(std::make_shared<Mesh>("res/uvcube.obj"));
	mRenderer.AddLight(light3->GetComponent("PointLight"));

}