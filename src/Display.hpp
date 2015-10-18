#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#include <SDL2/SDL.h>
#include <memory>
#include "Renderer.hpp"

class Display {

private:
	void InitGL();
	void SetViewport(int width, int height);
    void SetPerspective(int width, int height, float fov, bool usePerspective);

public:
    Display();
    
    SDL_Window* D_Window;
	std::unique_ptr<Renderer> renderer;

    bool Init();
    void Update(std::vector<std::shared_ptr<Entity> > entities);
    void Shutdown();
};

#endif
