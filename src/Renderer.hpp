#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include "lib/OpenGL.hpp"
#include "Entity.hpp"

class Renderer {
private:
public:
    Renderer();
    ~Renderer();

    bool Init();
	void RenderEntities(std::vector<Entity>* entities);
	void Shutdown();
};

#endif
