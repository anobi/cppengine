#ifndef G_GAME_H
#define G_GAME_H

typedef enum {
	GAMESTATE_RUNNING,
	GAMESTATE_STOPPED
} gameState_t;

class Game {

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
