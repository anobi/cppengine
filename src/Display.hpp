#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <memory>
#include <vector>

class Display {

private:

	SDL_GLContext _context;
	SDL_Window* _window;

	void InitGL();

public:
    Display();

    bool Init();
    void Update();
    void Shutdown();

	inline SDL_Window* GetWindow() { return this->_window; }
	inline const SDL_Window& GetWindow() const { return *this->_window; }
};

#endif
