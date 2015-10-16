#ifndef G_GAME_H
#define G_GAME_H

#include <memory>
#include <vector>
#include "Display.hpp"
#include "Input.hpp"
#include "Entity.hpp"

typedef enum {
	GAMESTATE_RUNNING,
	GAMESTATE_STOPPED
} gameState_t;

class Game {

private:
	Input input;
    std::unique_ptr<Display> display;
    std::vector<std::shared_ptr<Entity> > entities;

public:
	Game();

	gameState_t gameState;

	bool Init();
	void Start();
	void Shutdown();
	void Loop();
	void Quit();
};

#endif
