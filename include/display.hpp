#ifndef D_DISPLAY_H
#define D_DISPLAY_H

#include <SDL.h>
#include <memory>
#include <vector>

class Display {

private:

    SDL_GLContext _context;
    SDL_Window* _window;

    const char* _glsl_version;

    bool resizing;
    void InitGL();

public:
    Display() {};
    ~Display() {};

    bool Init(const int width, const int height);
    void Update();
    void SetResolution(const int width, const int height, bool fullScreen);
    void Shutdown();

    inline SDL_Window* GetWindow() { return this->_window; }
    inline const SDL_Window& GetWindow() const { return *this->_window; }

    inline SDL_GLContext* GetContext() { return &this->_context; }

    const char* GetGLSLVersion() { return this->_glsl_version; }

    inline float GetAspectRatio() { return (float)this->width / (float)this->height; }

    inline bool IsResizing() { return this->resizing; }

    int width = 800;
    int height = 600;
    int refresh_rate = 60;
};

#endif
