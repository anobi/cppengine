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

constexpr auto MAX_GAME_ENTITIES = 1024;

enum gameState_t
{
    GAMESTATE_RUNNING,
    GAMESTATE_STOPPED
};

class Game
{
public:
    Game();
    ~Game() {};

    gameState_t gameState;
    std::string workingDirectory;

    void AddEntity(Entity* entity);
    Entity* GetEntity(const std::string name);

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

    Scene* scene;
    Shader* shader;
    std::vector<Entity*> entities;

    void UpdateUI();
    void ConstructScene();

    bool menu = false;
    bool debug_ui = true;
};

#endif
