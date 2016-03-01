#ifndef G_GAME_H
#define G_GAME_H

#include <iostream>
#include <memory>
#include <vector>
#include "Display.hpp"
#include "Input.hpp"
#include "Controls.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"

typedef enum {
	GAMESTATE_RUNNING,
	GAMESTATE_STOPPED
} gameState_t;

class Game {

private:
	Input mInput;
	Controls mControls;
    Display mDisplay;
	Renderer mRenderer;
    std::vector<EntityRef> entities;

	void ConstructScene();

public:
	Game();

	gameState_t gameState;

	EntityRef AddEntity(EntityRef entity);
	EntityRef GetEntity(const std::string name);

    std::vector<EntityRef> GetEntities();

	bool Init();
	void Start();
	void Shutdown();
	void Loop();
	void Quit();
};

#endif
