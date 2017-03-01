#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

#include "lib/imgui.h"
#include "lib/imgui_internal.h"
#include "lib/imgui_impl_sdl_gl3.h"

#include "game.hpp"
#include "display.hpp"
#include "input.hpp"
#include "entity.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "material.hpp"

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
		mRenderer.UpdateResolution(mDisplay.GetWidth(), mDisplay.GetHeight());
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

	float ticks = 0;
	float counter = 0.0f;
	SDL_Event event;

    while(gameState == GAMESTATE_RUNNING) 
	{
		ImGui_ImplSdlGL3_NewFrame(mDisplay.GetWindow());
		
		/*************************
		* Update clock and so on *
		**************************/
		mRenderer.UpdateTick(ticks);

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
						mRenderer.UpdateResolution(mDisplay.GetWidth(), mDisplay.GetHeight());
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

		GetEntity("Fireball")->GetTransform().SetPosition(glm::fvec3(-7.5f, 10.0f, glm::cos(counter * 25) * 15));
		GetEntity("Lightningball")->GetTransform().SetPosition(glm::fvec3(7.5f, 2.5f, glm::cos(counter * 10) * -15));

		//TODO: figure out where to put this shit
		glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mRenderer.Render();

		UpdateUI();
		ImGui::Render();

		mDisplay.Update();

		counter += 0.001f;
		ticks += 1;
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

std::vector<std::shared_ptr<Entity>> Game::GetEntities()
{
    return entities;
};

void Game::AddEntity(std::shared_ptr<Entity> entity) 
{
	this->entities.push_back(entity);
}

std::shared_ptr<Entity> Game::GetEntity(const std::string name) 
{
	std::shared_ptr<Entity> entity = NULL;
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
	glm::fvec3 cRot = mRenderer.GetCamera()->mTransform.GetRotation();

	std::vector<std::string> entity_list;	
	
	for (int i = 0; i < entities.size(); i++) 
	{
		std::shared_ptr<Entity> e = entities[i];
		glm::fvec3 pos = e->GetTransform().GetPosition();

		char buf[256];
		snprintf(buf, 256, "%s", e->GetName().c_str(), pos.x, pos.y, pos.z);
		entity_list.push_back(buf);
	}

	// Debug info window
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, 80), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Info");
	ImGui::Text("Player position x: %.2f y: %.2f z: %.2f", cPos.x, cPos.y, cPos.z);
	ImGui::Text("Player rotation x: %.2f y: %.2f z: %.2f", cRot.x, cRot.y, cRot.z);
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

	camera->mTransform.SetPosition(glm::fvec3(0.0f, 2.0f, 0.0f));
	camera->mTransform.SetRotation(glm::fvec3(0.0f, 0.0f, 0.0f));
	mRenderer.SetCamera(camera);

	/*
	Meshes
	*/

	//Room
	std::shared_ptr<Entity> room = std::make_shared<Entity>(Entity("Room"));
	room->GetTransform().SetScale(glm::fvec3(0.015f));
	room->GetTransform().SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f));
	room->GetTransform().SetRotation(glm::fvec3(0.0f, glm::radians(90.0f), 0.0f));
	std::shared_ptr<Model> roomModel = std::make_shared<Model>("res/crytek-sponza/sponza.obj");
	mRenderer.AddModel(roomModel);
	room->AddComponent(roomModel);
	AddEntity(room);

	/*
	Lights
	*/

	//cool background light
	std::shared_ptr<Entity> light2 = std::make_shared<Entity>("Skylight");
	std::shared_ptr<DirectionalLight> pl2 = std::make_shared<DirectionalLight>(glm::fvec3(1.0f, 0.9f, 0.9f), 1.0f, 1.0);
	light2->GetTransform().SetPosition(glm::fvec3(1000.0f, 2000.0f, 500.0f));
	light2->AddComponent(pl2);
	AddEntity(light2);
	mRenderer.AddDirectionalLight(pl2);

	//awesome spinning FIRE BALL LIGHT YEAH
	std::shared_ptr<Entity> light3 = std::make_shared<Entity>("Fireball");
	std::shared_ptr<PointLight> pl3 = std::make_shared<PointLight>(glm::fvec3(1.0f, 0.4f, 0.0f), 1.0f, 0.1f, 5.0f);
	std::shared_ptr<Model> pl3model = std::make_shared<Model>("res/uvcube.obj");
	light3->GetTransform().SetPosition(glm::fvec3(-7.5f, 10.0f, 0.0f));
	light3->GetTransform().SetScale(glm::fvec3(0.1f));
	light3->AddComponent(pl3);
	light3->AddComponent(pl3model);
	AddEntity(light3);
	mRenderer.AddPointLight(pl3);
	mRenderer.AddModel(pl3model);

	std::shared_ptr<Entity> light4 = std::make_shared<Entity>("Lightningball");
	std::shared_ptr<PointLight> pl4 = std::make_shared<PointLight>(glm::fvec3(0.4f, 0.8f, 1.0f), 1.0f, 0.1f, 5.0f);
	light4->GetTransform().SetPosition(glm::fvec3(7.5f, 5.0f, 0.0f));
	light4->AddComponent(pl4);
	AddEntity(light4);
	mRenderer.AddPointLight(pl4);

}
