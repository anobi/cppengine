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

typedef enum gameState_t
{
	GAMESTATE_RUNNING,
	GAMESTATE_STOPPED
};

class Game 
{


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

private:
	Input mInput;
	Controls mControls;
	Display mDisplay;
	Renderer mRenderer;
	std::vector<EntityRef> entities;

	void UpdateUI();
	void ConstructScene();

	bool menu = false;
};

#endif
