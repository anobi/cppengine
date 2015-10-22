#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <memory>
#include "Renderer.hpp"

class Display {

private:

	SDL_GLContext _context;
	SDL_Window* _window;
	Renderer* _renderer;

	void InitGL();
	void SetViewport(int width, int height);
    void SetPerspective(int width, int height, float fov, bool usePerspective);

public:
    Display();

    bool Init(Renderer* renderer);
    void Update(std::vector<Entity>* entities);
    void Shutdown();
};

#endif
