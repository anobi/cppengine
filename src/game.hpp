#ifndef __GAME_H__
#define __GAME_H__

#include <iostream>
#include <memory>
#include <vector>

#include "constants.hpp"
#include "display.hpp"
#include "input.hpp"
#include "controls.hpp"
#include "renderer.hpp"
#include "world.hpp"

#include "entities/entity_manager.hpp"

#include "rendering/render_world.hpp"
#include "rendering/model_manager.hpp"
#include "rendering/material_manager.hpp"

enum class GAMESTATE
{
    RUNNING,
    STOPPED
};

class ModelLoader;

class Game
{
public:
    Game();
    ~Game() {};

    GAMESTATE gameState;
    std::string workingDirectory;

    bool Init();
    void Start();
    void Shutdown();
    void Loop();
    void Quit();

private:
    Input inputs;
    Controls controls;
    Display display;
    Renderer renderer;

    World world;

    Entities::EntityManager entity_manager;

    Rendering::MaterialManager material_manager;
    Rendering::ModelManager model_manager;

    RenderWorld ConstructRenderWorld();

    void UpdateUI();
    void ConstructScene(ModelLoader* modelLoader);


    bool menu = false;
    bool debug_ui = true;
};

#endif  // __GAME_H__
