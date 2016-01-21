#ifndef G_GAME_H
#define G_GAME_H

#include <memory>
#include <vector>
#include "Display.hpp"
#include "Input.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"

typedef enum {
	GAMESTATE_RUNNING,
	GAMESTATE_STOPPED
} gameState_t;

class Game {

private:
	Input mInput;
    Display mDisplay;
	Renderer mRenderer;

public:
	Game();

	gameState_t gameState;
    std::vector<Entity> entities;

	bool Init();
	void Start();
	void Shutdown();
	void Loop();
	void Quit();

    std::vector<Entity>* GetEntities();
};

#endif
