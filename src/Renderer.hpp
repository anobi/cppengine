#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include "Display.hpp"
#include "Entity.hpp"

class Renderer {
private:
    Display *display;
public:
    Renderer();
    ~Renderer();

    void Init(Display *display);
    void RenderEntity(renderEntity_t entity);
};

#endif
