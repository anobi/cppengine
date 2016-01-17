#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <memory>
#include <vector>
#include "Mesh.hpp"

class Display {

private:

	SDL_GLContext _context;
	SDL_Window* _window;

	void InitGL();
	void SetViewport(int width, int height);

public:
    Display();

    bool Init();
    void Update();
    void Shutdown();
};

#endif
