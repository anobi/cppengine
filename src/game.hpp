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

    void SetScene(std::shared_ptr<Scene> scene) { this->_scene = scene; }
    inline std::shared_ptr<Scene> GetScene() { return this->_scene; }

    void AddEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> GetEntity(const std::string name);
    std::vector<std::shared_ptr<Entity>> GetEntities();

    bool Init();
    void Start();
    void Shutdown();
    void Loop();
    void Quit();

    // :::::::::::::::::::::::::: Temp stuff :::::::::::::::::::::::::::::::

    // TODO: Move to model or texture or whatever should have the shader info.
    // Overridable by shader set in render pass.
    std::shared_ptr<Shader> shader;

    // ::::::::::::::::::::::: End of temp stuff :::::::::::::::::::::::::::

private:
    std::unique_ptr<Input> mInput;
    std::unique_ptr<Controls> mControls;
    std::unique_ptr<Display> mDisplay;
    std::unique_ptr<Renderer> mRenderer;

    std::shared_ptr<Scene> _scene;
    std::vector<std::shared_ptr<Entity>> entities;

    void UpdateUI();
    void ConstructScene();

    bool menu = false;
    bool debug_ui = true;
};

#endif
