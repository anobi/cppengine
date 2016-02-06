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

	int width;
	int height;

	void InitGL();

public:
    Display(){
		this->width = 800;
		this->height = 600;
	}

    bool Init(const int width, const int height);
    void Update();
	void SetResolution(const int width, const int height, bool fullScreen);
    void Shutdown();

	inline SDL_Window* GetWindow() { return this->_window; }
	inline const SDL_Window& GetWindow() const { return *this->_window; }

	inline int GetWidth() { return this->width; }
	inline int GetHeight() { return this->height; }
	inline float GetAspectRatio() {return (float)this->width / (float)this->height;}
};

#endif
