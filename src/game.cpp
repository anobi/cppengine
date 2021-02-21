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


// TODO: Temp holders. Create new homes for these.
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
    SDL_Event event;
    int frames = 0;
    int ticks = 0;

    // float target_framerate = 60.0f;
    // float target_framerate = 90.0f;
    // float target_framerate = 144.0f;
    float target_framerate = 200.0f;

    float target_frame_time = 1000.0f / target_framerate;
    Uint64 loop_start = SDL_GetPerformanceCounter();

    while (gameState == GAMESTATE_RUNNING)
    {
        Uint64 loop_end = SDL_GetPerformanceCounter();
        Uint64 frame_time = loop_end - loop_start;
        loop_start = loop_end;

        float elapsed = frame_time / (float)SDL_GetPerformanceFrequency();
        float delay = target_frame_time - elapsed;
        if (elapsed >= 0) {
            SDL_Delay(delay);
        }

        ticks++;
        this->renderer.UpdateTick(ticks);

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
                    this->scene->camera->SetAspectRatio(this->display.GetAspectRatio());
                    break;
                }
            }
        }

        //float dd = 1 / delay;
        if (!menu) {
            this->controls.Update(event, this->scene->camera, delay);
        }
        
        // Animate the light cubes
        float dt = 1.0f / target_framerate;
        float e1_speed = 0.25f;
        float e2_speed = 0.10f;
        float e1_pos = glm::cos((ticks + e1_speed) * dt) * 15;
        float e2_pos = glm::cos((ticks + e2_speed) * dt) * 15;
        GetEntity("Fireball")->GetTransform().SetPosition(glm::fvec3(-7.5f, 10.0f, e1_pos));
        GetEntity("Lightningball")->GetTransform().SetPosition(glm::fvec3(7.5f, 2.5f, e2_pos));

        //TODO: figure out where to put this shit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->renderer.Render(this->scene, this->shader);

        if (this->debug_ui)
        {
            ImGui_ImplSdlGL3_NewFrame(this->display.GetWindow());
            UpdateUI();
            ImGui::Render();
        }

        this->display.Update();
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
    glm::fvec3 cPos = this->scene->camera->transform.GetPosition();
    glm::fvec3 cRot = this->scene->camera->transform.GetRotation();

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

    // Settings window
    ImGui::SetNextWindowPos(ImVec2(10, 1000), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Settings");

    ImGui::DragFloat("Mouse sensitivity", &this->controls.mouseSensitivity);

    ImGui::End();
}

void Game::ConstructScene()
{
    std::cout << "Loading game content..." << std::endl;
    std::cout << "--------------------" << std::endl;

    defaultScene = Scene();
    this->scene = &defaultScene;

    defaultShader = Shader("default");
    this->shader = &defaultShader;

    camera = Camera(
        60.0f,  // FOV
        0.1f,   // zNear
        1000.0f  // zFar
    );

    camera.transform.SetPosition(glm::fvec3(0.0f, 2.0f, 0.0f));
    camera.transform.SetRotation(glm::fvec3(0.0f, 0.0f, 0.0f));
    camera.SetAspectRatio(this->display.GetAspectRatio());
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
