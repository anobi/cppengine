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

	void AddEntity(std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> GetEntity(const std::string name);
	std::vector<std::shared_ptr<Entity>> GetEntities();

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
	std::vector<std::shared_ptr<Entity>> entities;

	void UpdateUI();
	void ConstructScene();

	bool menu = false;
	bool debug_ui = true;
};

#endif
