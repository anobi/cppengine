#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include "lib/OpenGL.hpp"
#include "Entity.hpp"

class Renderer {
private:
public:
    Renderer();
    ~Renderer();

    bool Init();
	void RenderEntities(std::vector<Entity>* entities);
    void RenderEntity(renderEntity_t entity, int* pointcount);
	void Shutdown();
};

#endif
