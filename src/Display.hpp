#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#include <SDL2/SDL.h>

class Display {

private:
	void InitGL();
	void SetViewport(int width, int height);
    void SetPerspective(int width, int height, float fov, bool usePerspective);

public:
    Display();
    
    SDL_Window* D_Window;
    SDL_Renderer* D_Renderer;
	SDL_RendererInfo D_RendererInfo;

    bool Init();
    void Update();
    void Shutdown();
};

#endif
