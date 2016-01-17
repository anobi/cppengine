#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <memory>
#include <vector>
#include "Renderer.hpp"
#include "Mesh.hpp"

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
    void Update(Mesh* mesh);
    void Shutdown();
};

#endif
