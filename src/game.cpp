#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

#include "lib/imgui.h"
#include "lib/imgui_internal.h"
#include "lib/imgui_impl_sdl_gl3.h"

#include "configuration.hpp"
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
    std::cout << "Initializing game..." << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "* Working directory: " << Configuration::Get().workingDirectory << std::endl;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL Error: %s\n", SDL_GetError();
        return false;
    }

    //Display
    std::cout << "* Display: ";
    if (!this->display.Init(1440, 900))
    {
        std::cout << "Error: %s\n", SDL_GetError();
        return false;
    }
    else std::cout << "done" << std::endl;

    //Renderer
    std::cout << "* Renderer: ";
    if (!this->renderer.Init())
    {
        std::cout << "Error: %s\n", SDL_GetError();
        return false;
    }
    else std::cout << "done" << std::endl;

    //Input system
    std::cout << "* Input: ";
    if (!this->inputs.Init())
    {
        std::cout << "Error: %s\n", SDL_GetError();
        return false;
    }
    else std::cout << "done" << std::endl;

    ImGui_ImplSdlGL3_Init(this->display.GetWindow());

    SDL_SetRelativeMouseMode(SDL_TRUE);
    this->controls.SetSensitivity(0.00025f);
    this->controls.ResetMousePosition(this->display.GetWindow(), this->display.width / 2, this->display.height / 2);

    std::cout << std::endl;
    return true;
}

void Game::Start()
{

    //Init everything
    if (Init())
    {
        this->renderer.UpdateResolution(this->display.width, this->display.height);
        //Build the scene, a temp solution
        ConstructScene();

        //Start the game loop
        gameState = GAMESTATE_RUNNING;
        Loop();
    }
    else
    {
        std::cout << "  !! ERROR: Failed to initialize game" << std::endl;
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

    while (gameState == GAMESTATE_RUNNING)
    {

        /*************************
        * Update clock and so on *
        **************************/
        this->renderer.UpdateTick(ticks);

        auto loop_start = high_resolution_clock::now();
        auto loop_end = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(loop_end - loop_start);
        auto delay = ((milliseconds)1000 / 60 - elapsed).count();

        // if(delay >= 0) SDL_Delay(delay);

        /*****************************
        * Handle controls and events *
        ******************************/

        while (SDL_PollEvent(&event))
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

                case SDLK_LALT:
                    menu = !menu;
                    SDL_SetRelativeMouseMode((SDL_bool)!menu);
                    this->controls.ResetMousePosition(this->display.GetWindow(), this->display.width / 2, this->display.height / 2);
                    SDL_ShowCursor(menu);
                    break;

                case SDLK_F1:
                    debug_ui = !debug_ui;
                    break;
                }
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    this->display.SetResolution(event.window.data1, event.window.data2, true);
                    this->renderer.UpdateResolution(this->display.width, this->display.height);
                    this->scene->camera->SetAspectRatio(45.0f, this->display.GetAspectRatio());
                    break;
                }
            }
        }

        if (!menu) {
            this->controls.Update(event, this->scene->camera, delay);
        }

        /*****************************
        * Update entities and render *
        ******************************/

        GetEntity("Fireball")->GetTransform().SetPosition(glm::fvec3(-7.5f, 10.0f, glm::cos(counter * 25) * 15));
        GetEntity("Lightningball")->GetTransform().SetPosition(glm::fvec3(7.5f, 2.5f, glm::cos(counter * 10) * -15));

        //TODO: figure out where to put this shit
        glClearColor(0.1f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->renderer.Render(this->scene, this->shader);

        if (this->debug_ui)
        {
            ImGui_ImplSdlGL3_NewFrame(this->display.GetWindow());
            UpdateUI();
            ImGui::Render();
        }

        this->display.Update();

        counter += 0.001f;
        ticks += 1;
    }

    Shutdown();
}

void Game::Shutdown()
{
    std::cout << "Shutting down...\n";
    std::cout << "--------------------" << std::endl;

    ImGui::Shutdown();

    std::cout << "* Input\n";
    this->inputs.Shutdown();

    std::cout << "* Renderer\n";
    this->renderer.Shutdown();

    std::cout << "* Display\n";
    this->display.Shutdown();

    SDL_Quit();
}

void Game::Quit()
{
    std::cout << "QUIT\n";
    gameState = GAMESTATE_STOPPED;
}

std::vector<Entity*> Game::GetEntities()
{
    return entities;
};

void Game::AddEntity(Entity* entity)
{
    this->entities.push_back(entity);
}

Entity* Game::GetEntity(const std::string name)
{
    Entity* entity = NULL;

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

    if (idx < 0 || idx >= static_cast<int>(vector.size()))
    {
        return false;
    }

    *out_text = vector.at(idx).c_str();

    return true;
};

void Game::UpdateUI()
{
    glm::fvec3 cPos = this->scene->camera->GetPosition();
    glm::fvec3 cRot = this->scene->camera->mTransform.GetRotation();

    std::vector<std::string> entity_list;

    for (int i = 0; i < entities.size(); i++)
    {
        Entity* e = entities[i];
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


// Temp holders
Scene defaultScene;
Shader defaultShader;
Camera camera;
Entity room;

Model roomModel;
Model pl1model;
Model pl2model;
Model pl3model;
Model pl4model;

Entity light1;
Entity light2;
Entity light3;
Entity light4;

DirectionalLight pl2;

PointLight pl1;
PointLight pl3;
PointLight pl4;

void Game::ConstructScene()
{
    std::cout << "Loading game content..." << std::endl;
    std::cout << "--------------------" << std::endl;

    defaultScene = Scene();
    this->scene = &defaultScene;

    defaultShader = Shader("default");
    this->shader = &defaultShader;

    camera = Camera(Camera(glm::perspective(
        45.0f, //FOV
        this->display.GetAspectRatio(), //duh
        0.1f, //depth aka znear
        100.0f))); //zFar

    // TODO: Make a scene object that contains cameras and shite
    // They don't belong in renderer
    camera.mTransform.SetPosition(glm::fvec3(0.0f, 2.0f, 0.0f));
    camera.mTransform.SetRotation(glm::fvec3(0.0f, 0.0f, 0.0f));
    this->scene->camera = &camera;

    /*
    Meshes
    */

    //Room
    room = Entity(Entity("Room"));
    room.GetTransform().SetScale(glm::fvec3(0.02f));
    room.GetTransform().SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f));
    room.GetTransform().SetRotation(glm::fvec3(0.0f, glm::radians(90.0f), 0.0f));
    roomModel = Model("sponza.obj");
    room.AddComponent(&roomModel);

    AddEntity(&room);
    this->scene->AddModel(&roomModel);

    /*
    Lights
    */

    //cool background light
    light2 = Entity("Skylight");
    pl2 = DirectionalLight(glm::fvec3(1.0f, 0.9f, 0.9f), 1.0f, 1.0);
    light2.GetTransform().SetPosition(glm::fvec3(1000.0f, 2000.0f, 500.0f));
    light2.AddComponent(&pl2);
    AddEntity(&light2);
    this->scene->AddDirectionalLight(&pl2);

    //awesome spinning FIRE BALL LIGHT YEAH
    light3 = Entity("Fireball");
    pl3 = PointLight(glm::fvec3(1.0f, 0.4f, 0.0f), 1.0f, 0.1f, 5.0f);
    pl3model = Model("uvcube.obj");
    light3.GetTransform().SetPosition(glm::fvec3(-7.5f, 10.0f, 0.0f));
    light3.GetTransform().SetScale(glm::fvec3(0.1f));
    light3.AddComponent(&pl3);
    light3.AddComponent(&pl3model);

    AddEntity(&light3);
    this->scene->AddPointLight(&pl3);
    this->scene->AddModel(&pl3model);


    light4 = Entity("Lightningball");
    pl4 = PointLight(glm::fvec3(0.4f, 0.8f, 1.0f), 1.0f, 0.1f, 5.0f);
    pl4model = Model("uvcube.obj");
    light4.GetTransform().SetPosition(glm::fvec3(7.5f, 5.0f, 0.0f));
    light4.GetTransform().SetScale(glm::fvec3(0.1f));
    light4.AddComponent(&pl4);
    light4.AddComponent(&pl4model);

    AddEntity(&light4);
    this->scene->AddPointLight(&pl4);
    this->scene->AddModel(&pl4model);

    assert(this->scene);
    assert(this->scene->camera);

    // Done loading, print empty line
    std::cout << std::endl;
}
