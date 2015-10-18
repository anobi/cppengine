#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include "Entity.hpp"

class Renderer {
private:
public:
    Renderer();
    ~Renderer();

    void Init();
    void RenderEntity(renderEntity_t entity);
};

#endif
