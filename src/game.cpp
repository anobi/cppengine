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
#include "shader.hpp"
#include "loading/model_loader.hpp"

#include "containers/array.hpp"


// TODO: Temp holders. Create new homes for these.
Shader defaultShader;
Shader greyboxShader;
Camera camera;

entityHandle_t test_cube;
entityHandle_t orange_light;
entityHandle_t blue_light;

Game::Game()
{
    this->gameState = GAMESTATE::STOPPED;
}


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

        this->renderer.material_manager = &this->material_manager;
        this->renderer.model_manager = &this->model_manager;

        this->world.entity_manager = &this->entity_manager;
        this->world.model_manager = &this->model_manager;
        this->world.material_manager = &this->material_manager;

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

    //float target_framerate = 60.0f;
    // float target_framerate = 90.0f;
    // float target_framerate = 144.0f;
    float target_framerate = 200.0f;
    float target_frame_time = 1000.0f / target_framerate;
    Uint64 loop_start = SDL_GetPerformanceCounter();

    while (this->gameState == GAMESTATE::RUNNING)
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
                    this->debug_ui = !debug_ui;
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
                    this->world.camera->SetAspectRatio(this->display.GetAspectRatio());
                    break;
                }
            }
        }

        bool dirty_camera = false;
        if (!this->menu) {
            dirty_camera = this->controls.Update(event, this->world.camera, delay);
        }

        //TODO: figure out where to put this shit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Animate the light cubes
        float dt = 1.0f / target_framerate;
        float e1_speed = 0.25f;
        float e2_speed = 0.10f;
        float e1_pos = glm::cos((ticks + e1_speed) * dt) * 15;
        float e2_pos = glm::cos((ticks + e2_speed) * dt) * 15;

        // auto orange_light = this->entity_manager.Find("Orange light");
        // auto blue_light = this->entity_manager.Find("Blue light");
        // auto test_cube = this->entity_manager.Find("Test cube");
        this->entity_manager.SetPosition(orange_light, glm::fvec3(-7.5f, 10.0f, e1_pos));
        this->entity_manager.SetPosition(blue_light, glm::fvec3(7.5f, 2.5f, e2_pos));
        this->entity_manager.SetRotation(test_cube, glm::fvec3(
            1.0f + ticks * dt,
            1.0f + ticks * dt,
            0.0f
        ));

        this->entity_manager.spatial_components.Update(this->world.camera->GetViewProjection(), dirty_camera);

        RenderWorld render_world = this->ConstructRenderWorld();
        // RenderWorld render_world;
        this->renderer.Render(render_world, &defaultShader);


        if (this->debug_ui)
        {
            ImGui_ImplSdlGL3_NewFrame(this->display.GetWindow());
            UpdateUI();
            ImGui::Render();
        }

        this->display.Update();
    }

    // Tear down the scene
    this->world.Cleanup();

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

RenderWorld Game::ConstructRenderWorld()
{
    RenderWorld render_world;

    render_world.render_resolution  = this->renderer.GetResolution();
    render_world.view_matrix        = this->world.camera->GetView();
    render_world.projection_matrix  = this->world.camera->GetProjection();
    render_world.eye_position       = this->world.camera->transform.GetPosition();

    // Set lights
    for (int i = 0; i < this->entity_manager.light_components._entities_top; i++) 
    {
        entitySlot_t light = this->entity_manager.light_components._entity_index[i];
        lightTypes light_type = this->entity_manager.light_components.light_types[light.slot];

        render_world.light_types[i]         = light_type;
        render_world.light_colors[i]        = this->entity_manager.light_components.colors[light.slot];
        render_world.light_intensities[i]   = this->entity_manager.light_components.intensities[light.slot];
        render_world.light_positions[i]     = this->entity_manager.spatial_components.GetPosition(light.entity);

        if (light_type == lightTypes::POINTLIGHT) {
            render_world.light_radiuses[i]  = this->entity_manager.light_components.radiuses[light.slot];
            render_world.light_cutoffs[i]   = this->entity_manager.light_components.cutoffs[light.slot];
        }
        render_world.light_count += 1;
    }

    for (int i = 0; i < this->entity_manager.model_components._entities_top; i++) {
        entitySlot_t model              = this->entity_manager.model_components._entity_index[i];
        render_world.materials[i]       = this->entity_manager.model_components.materials[model.slot];

        entitySlot_t model_resource     = this->model_manager.FindResource(model.entity);
        render_world.VAOs[i]            = this->model_manager.VAOs[model_resource.slot];
        render_world.indices[i]         = this->model_manager.indices[model_resource.slot];

        entitySlot_t spatial_resource   = this->entity_manager.spatial_components.FindResource(model.entity);
        render_world.model_matrices[i]  = this->entity_manager.spatial_components.model_matrices[spatial_resource.slot];
        render_world.normal_matrices[i] = this->entity_manager.spatial_components.normal_matrices[spatial_resource.slot];

        render_world.render_entity_count += 1;
    }

    return render_world;
    
}

static auto entity_array_getter = [](void* entities, int idx, const char** out_text)
{
    Array<entitySlot_t, MAX_GAME_ENTITIES> entity_list = *static_cast<Array<entitySlot_t, MAX_GAME_ENTITIES>*>(entities);
    if (idx < 0 || idx >= static_cast<int>(entity_list.Size()))
    {
        return false;
    }
    *out_text = entity_list[idx].name.c_str();
    return true;
};

int selected_entity = 0;
void Game::UpdateUI()
{
    // Debug info window
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 80), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Info");

    glm::fvec3 cPos = this->world.camera->transform.GetPosition();
    glm::fvec3 cRot = this->world.camera->transform.GetRotation();
    ImGui::Text("Player position x: %.2f y: %.2f z: %.2f", cPos.x, cPos.y, cPos.z);
    ImGui::Text("Player rotation x: %.2f y: %.2f z: %.2f", cRot.x, cRot.y, cRot.z);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // Entity debug window
    ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Entities");

    ImGui::PushItemWidth(-1);
    ImGui::ListBox(
        "##entities", 
        &selected_entity, 
        entity_array_getter, 
        static_cast<void*>(&this->entity_manager._entity_index), 
        this->entity_manager._entities_top,
        32
    );
    ImGui::PopItemWidth();

    entityHandle_t selected = this->entity_manager._entity_index[selected_entity].entity;
    glm::fvec3 e_pos = this->entity_manager.spatial_components.GetPosition(selected);
    ImGui::Text("Location x: %.2f y: %.2f z:%.2f", e_pos.x, e_pos.y, e_pos.z);
    ImGui::SliderFloat("X", &e_pos.x, -100, 100);
    ImGui::SliderFloat("Y", &e_pos.y, -100, 100);
    ImGui::SliderFloat("Z", &e_pos.z, -100, 100);

    this->entity_manager.SetPosition(selected, e_pos);

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

    defaultShader = Shader("default");
    greyboxShader = Shader("greybox");

    camera = Camera(
        60.0f,  // FOV
        0.1f,   // zNear
        1000.0f  // zFar
    );

    camera.transform.SetPosition(glm::fvec3(0.0f, 2.0f, 0.0f));
    camera.transform.SetRotation(glm::fvec3(0.0f, 0.0f, 0.0f));
    camera.SetAspectRatio(this->display.GetAspectRatio());
    this->world.camera = &camera;

    /*
    Meshes
    */

    // Even more temp test solution for a data oriented test cube
     test_cube = this->entity_manager.Add("Test cube");
     this->world.AddEntity(test_cube);

     this->entity_manager.SetPosition(test_cube, glm::fvec3(0.0f, 3.0f, 0.0f));
     LOADINGSTATE test_cube_load = modelLoader->Load("uvcube.obj", test_cube);
     assert(test_cube_load == LOADINGSTATE::VALID);
     
    
    //Room
    auto room = this->entity_manager.Add("Room");
    this->entity_manager.SetScale(room, glm::fvec3(0.02f));
    this->entity_manager.SetRotation(room, glm::fvec3(0.0f, glm::radians(90.0f), 0.0f));
    this->world.AddEntity(room);

    LOADINGSTATE room_load = modelLoader->Load("sponza.obj", room);
    assert(room_load == LOADINGSTATE::VALID);

    /*
    Lights
    */

    //cool background light
    auto bg_light = this->entity_manager.Add("Background light");
    this->world.AddEntity(bg_light);
    this->entity_manager.SetPosition(bg_light, glm::fvec3(50.0f, 30.0f, -10.0f));
    this->entity_manager.AddDirectionalLight(
        bg_light,
        { 
            { 1.0f, 1.0f, glm::fvec3(1.0f, 0.9f, 0.9f) },
            glm::fvec3(0.0f, 0.0f, 0.0f)
        });


    // Fiery light cube
    orange_light = this->entity_manager.Add("Orange light");
    this->world.AddEntity(orange_light);

    modelLoader->Load("uvcube.obj", orange_light);
    this->entity_manager.SetPosition(orange_light, glm::fvec3(-7.5f, 10.0f, 0.0f));
    this->entity_manager.SetScale(orange_light, glm::fvec3(0.1f));
    this->entity_manager.AddPointLight(
        orange_light,
        { 
            { 1.0f, 0.1f, glm::fvec3(1.0f, 0.4f, 0.0f) },
            5.0f 
        });
    

    // Blue light cube
    blue_light = this->entity_manager.Add("Blue light");
    this->world.AddEntity(blue_light);

    modelLoader->Load("uvcube.obj", blue_light);
    this->entity_manager.SetPosition(blue_light, glm::fvec3(7.5f, 2.5f, 0.0f));
    this->entity_manager.SetScale(blue_light, glm::fvec3(0.1f));
    this->entity_manager.AddPointLight(
        blue_light,
        {
            { 1.0f, 0.1f, glm::fvec3(0.4f, 0.8f, 1.0f) }, 
            5.0f
        });

    // Done loading
    printf("\n");
}