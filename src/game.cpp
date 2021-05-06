#include <cstdio>
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
#include "loading/model_loader.hpp"

#include "containers/array.hpp"


// TODO: Temp holders. Create new homes for these.
Scene defaultScene;
Shader defaultShader;
Camera camera;

Entity room = Entity("Room");
Entity bg_light = Entity("Skylight");
Entity orange_light = Entity("Fireball");
Entity blue_light = Entity("Lightningball");

Entity test_cube = Entity("TestCube");
Model test_cube_model;

Model roomModel;
Model pl1model;
Model pl2model;
Model pl3model;
Model pl4model;

DirectionalLight pl2;

PointLight pl1;
PointLight pl3;
PointLight pl4;


Game::Game()
{
    this->gameState = GAMESTATE::STOPPED;
}

int selected_entity = 0;

bool Game::Init()
{
    const char* cwd = Configuration::Get().workingDirectory.c_str();
    printf("Initializing game... \n");
    printf("-------------------- \n");
    printf("* Working directory: %s \n", cwd);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL Error: %s \n", SDL_GetError());
        return false;
    }

    //Display
    printf("* Display: ");
    if (!this->display.Init(1440, 900))
    {
        printf("Error: %s \n", SDL_GetError());
        return false;
    }
    else printf("done \n");

    //Renderer
    printf("* Renderer: ");
    if (!this->renderer.Init())
    {
        printf("Error: %s \n", SDL_GetError());
        return false;
    }
    else printf("done \n");

    //Input system
    printf("* Input: ");
    if (!this->inputs.Init())
    {
        printf("Error: %s \n", SDL_GetError());
        return false;
    }
    else printf("done \n");

    ImGui_ImplSdlGL3_Init(this->display.GetWindow());

    SDL_SetRelativeMouseMode(SDL_TRUE);
    this->controls.ResetMousePosition(this->display.GetWindow(), this->display.width / 2, this->display.height / 2);

    printf("\n");
    return true;
}

void Game::Start()
{
    //Init everything
    if (this->Init())
    {
        this->renderer.UpdateResolution(this->display.width, this->display.height);

        //Build the scene, a temp solution
        ModelLoader modelLoader = ModelLoader(&this->world);
        this->ConstructScene(&modelLoader);

        //Start the game loop
        this->gameState = GAMESTATE::RUNNING;
        this->Loop();
    }
    else
    {
        printf("  !! ERROR: Failed to initialize game \n");
        exit(EXIT_FAILURE);
    }
}

void Game::Loop()
{
    SDL_Event event;
    int ticks = 0;

    // float target_framerate = 60.0f;
    // float target_framerate = 90.0f;
    // float target_framerate = 144.0f;
    float target_framerate = 200.0f;
    float target_frame_time = 1000.0f / target_framerate;
    Uint64 loop_start = SDL_GetPerformanceCounter();

    bool dod = false;  // Test switch between the old object oriented and new data oriented entities

    while (this->gameState == GAMESTATE::RUNNING)
    {
        Uint64 loop_end = SDL_GetPerformanceCounter();
        Uint64 frame_time = loop_end - loop_start;
        loop_start = loop_end;

        // TODO: Replace the delay with a spinlock
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
                    this->debug_ui = !debug_ui;
                    break;

                case SDLK_TAB:
                    dod = !dod;
                    break;

                default:
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

        if (!this->menu) {
            this->controls.Update(event, this->scene->camera, delay);
        }

        //TODO: figure out where to put this shit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Animate the light cubes
        float dt = 1.0f / target_framerate;
        float e1_speed = 0.25f;
        float e2_speed = 0.10f;
        float e1_pos = glm::cos((ticks + e1_speed) * dt) * 15;
        float e2_pos = glm::cos((ticks + e2_speed) * dt) * 15;

        if (dod) 
        {
            this->world.entity_transforms.SetPosition(orange_light.handle, glm::fvec3(-7.5f, 10.0f, e1_pos));
            this->world.entity_transforms.SetPosition(blue_light.handle, glm::fvec3(7.5f, 2.5f, e2_pos));

            this->world.entity_transforms.Update(this->scene->camera->GetViewProjection());
            this->renderer.Render(&this->world, this->shader);
        }
        else 
        {
            this->GetEntity("Fireball")->transform.SetPosition(glm::fvec3(-7.5f, 10.0f, e1_pos));
            this->GetEntity("Lightningball")->transform.SetPosition(glm::fvec3(7.5f, 2.5f, e2_pos));

            this->renderer.Render(this->scene, this->shader);
        }

        if (this->debug_ui)
        {
            ImGui_ImplSdlGL3_NewFrame(this->display.GetWindow());
            UpdateUI();
            ImGui::Render();
        }

        this->display.Update();
    }

    // Tear down the scene
    this->shader->Cleanup();
    this->shader = 0;

    this->scene->Cleanup();
    this->scene = 0;

    Shutdown();
}

void Game::Shutdown()
{
    printf("Shutting down... \n");
    printf("-------------------- \n");

    ImGui::Shutdown();

    printf("* Input \n");
    this->inputs.Shutdown();

    printf("* Renderer \n");
    this->renderer.Shutdown();

    printf("* Display \n");
    this->display.Shutdown();

    SDL_Quit();
}

void Game::Quit()
{
    printf("QUIT \n");
    gameState = GAMESTATE::STOPPED;
}

void Game::AddEntity(Entity* entity)
{
    this->entities.push_back(entity);
}

Entity* Game::GetEntity(const std::string name)
{
    Entity* entity = NULL;

    for (unsigned int i = 0; i < this->entities.size(); i++)
    {
        if (this->entities[i]->name == name)
        {
            entity = this->entities[i];
            break;
        }
    }

    return entity;
}

static auto entity_array_getter = [](void* entities, int idx, const char** out_text)
{
    Array<char*, MAX_GAME_ENTITIES> entity_list = *static_cast<Array<char*, MAX_GAME_ENTITIES>*>(entities);
    if (idx < 0 || idx >= static_cast<int>(entity_list.Size()))
    {
        return false;
    }
    *out_text = entity_list[idx];
    return true;
};

void Game::UpdateUI()
{
    Array<const char*, MAX_GAME_ENTITIES> entity_list;
    int num_entities = this->entities.size();
    if (num_entities > MAX_GAME_ENTITIES) {
        num_entities = MAX_GAME_ENTITIES;
    }

    for (int i = 0; i < num_entities; i++)
    {
        entity_list[i] = this->entities[i]->name;
    }

    // Debug info window
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 80), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Info");

    glm::fvec3 cPos = this->scene->camera->transform.GetPosition();
    glm::fvec3 cRot = this->scene->camera->transform.GetRotation();
    ImGui::Text("Player position x: %.2f y: %.2f z: %.2f", cPos.x, cPos.y, cPos.z);
    ImGui::Text("Player rotation x: %.2f y: %.2f z: %.2f", cRot.x, cRot.y, cRot.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // Entity debug window
    ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Entities");

    ImGui::PushItemWidth(-1);
    ImGui::ListBox("##entities", &selected_entity, entity_array_getter, static_cast<void*>(&entity_list), num_entities);
    ImGui::PopItemWidth();

    glm::fvec3 e_pos = entities[selected_entity]->transform.GetPosition();
    ImGui::Text("Location x: %.2f y: %.2f z:%.2f", e_pos.x, e_pos.y, e_pos.z);
    ImGui::SliderFloat("X", &e_pos.x, -100, 100);
    ImGui::SliderFloat("Y", &e_pos.y, -100, 100);
    ImGui::SliderFloat("Z", &e_pos.z, -100, 100);

    this->entities[selected_entity]->transform.SetPosition(e_pos);

    ImGui::End();

    // Settings window
    ImGui::SetNextWindowPos(ImVec2(10, 1000), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Settings");

    ImGui::DragFloat("Mouse sensitivity", &this->controls.mouseSensitivity, 0.1f, 0.0f, 10.0f);

    ImGui::End();
}

void Game::ConstructScene(ModelLoader* modelLoader)
{
    printf("Loading game content... \n");
    printf("-------------------- \n");

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
    this->world.camera = &camera;

    /*
    Meshes
    */

    // Even more temp test solution for a data oriented test cube
    test_cube.handle = this->world.AddEntity(&test_cube);
    LOADINGSTATE test_cube_load = modelLoader->Load("uvcube.obj", &test_cube_model, test_cube.handle);
    assert(test_cube_load == LOADINGSTATE::VALID);

    this->world.render_entities.Add(test_cube.handle);
    this->world.entity_transforms.Add(test_cube.handle);

    //Room
    room.handle = this->world.AddEntity(&room);
    LOADINGSTATE room_load = modelLoader->Load("sponza.obj", &roomModel, room.handle);
    assert(room_load == LOADINGSTATE::VALID);

    // dod
    this->world.render_entities.Add(room.handle);
    this->world.entity_transforms.Add(room.handle);
    this->world.entity_transforms.SetScale(room.handle, glm::fvec3(0.02f));
    this->world.entity_transforms.SetRotation(room.handle, glm::fvec3(0.0f, glm::radians(90.0f), 0.0f));

    // oop
    room.transform.SetScale(glm::fvec3(0.02f));
    room.transform.SetPosition(glm::fvec3(0.0f, 0.0f, 0.0f));
    room.transform.SetRotation(glm::fvec3(0.0f, glm::radians(90.0f), 0.0f));
    room.AddComponent(&roomModel);
    AddEntity(&room);
    this->scene->AddModel(&roomModel);

    /*
    Lights
    */

    //cool background light
    bg_light.handle = this->world.AddEntity(&bg_light);

    pl2 = DirectionalLight(glm::fvec3(1.0f, 0.9f, 0.9f), 1.0f, 1.0);
    bg_light.transform.SetPosition(glm::fvec3(1000.0f, 2000.0f, 500.0f));
    bg_light.AddComponent(&pl2);
    AddEntity(&bg_light);
    this->scene->AddDirectionalLight(&pl2);


    // Fiery light cube
    orange_light.handle = this->world.AddEntity(&orange_light);
    modelLoader->Load("uvcube.obj", &pl3model, orange_light.handle);

    this->world.render_entities.Add(orange_light.handle);
    this->world.entity_transforms.Add(orange_light.handle);
    this->world.entity_transforms.SetPosition(orange_light.handle, glm::fvec3(-7.5f, 10.0f, 0.0f));
    this->world.entity_transforms.SetScale(orange_light.handle, glm::fvec3(0.1f));

    orange_light.transform.SetPosition(glm::fvec3(-7.5f, 10.0f, 0.0f));
    orange_light.transform.SetScale(glm::fvec3(0.1f));

    pl3 = PointLight(glm::fvec3(1.0f, 0.4f, 0.0f), 1.0f, 0.1f, 5.0f);
    orange_light.AddComponent(&pl3);
    orange_light.AddComponent(&pl3model);

    AddEntity(&orange_light);
    this->scene->AddPointLight(&pl3);
    this->scene->AddModel(&pl3model);


    // Blue light cube
    blue_light.handle = this->world.AddEntity(&blue_light);
    modelLoader->Load("uvcube.obj", &pl4model, blue_light.handle);

    this->world.render_entities.Add(blue_light.handle);
    this->world.entity_transforms.Add(blue_light.handle);
    this->world.entity_transforms.SetPosition(blue_light.handle, glm::fvec3(7.5f, 2.5f, 0.0f));
    this->world.entity_transforms.SetScale(blue_light.handle, glm::fvec3(0.1f));

    blue_light.transform.SetPosition(glm::fvec3(7.5f, 5.0f, 0.0f));
    blue_light.transform.SetScale(glm::fvec3(0.1f));

    pl4 = PointLight(glm::fvec3(0.4f, 0.8f, 1.0f), 1.0f, 0.1f, 5.0f);
    blue_light.AddComponent(&pl4);
    blue_light.AddComponent(&pl4model);

    AddEntity(&blue_light);
    this->scene->AddPointLight(&pl4);
    this->scene->AddModel(&pl4model);


    // Done loading
    assert(this->scene);
    assert(this->scene->camera);
    printf("\n");
}
