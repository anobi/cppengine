#ifndef G_GAME_H
#define G_GAME_H

#include <iostream>
#include <memory>
#include <vector>

#include "display.hpp"
#include "input.hpp"
#include "controls.hpp"
#include "entity.hpp"
#include "renderer.hpp"
#include "scene.hpp"

enum gameState_t
{
    GAMESTATE_RUNNING,
    GAMESTATE_STOPPED
};

class Game
{


public:
    Game();

    gameState_t gameState;
    std::string workingDirectory;

    void AddEntity(Entity* entity);
    Entity* GetEntity(const std::string name);
    std::vector<Entity*> GetEntities();

    bool Init();
    void Start();
    void Shutdown();
    void Loop();
    void Quit();

    // :::::::::::::::::::::::::: Temp stuff :::::::::::::::::::::::::::::::

    // TODO: Move to model or texture or whatever should have the shader info.
    // Overridable by shader set in render pass.
    

    // ::::::::::::::::::::::: End of temp stuff :::::::::::::::::::::::::::

private:
    Input inputs;
    Controls controls;
    Display display;
    Renderer renderer;

    Scene* scene;
    Shader* shader;
    std::vector<Entity*> entities;

    void UpdateUI();
    void ConstructScene();

    bool menu = false;
    bool debug_ui = true;
};

#endif
