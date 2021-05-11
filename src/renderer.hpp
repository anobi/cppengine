#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <memory>

#include "display.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "shader.hpp"

#include "rendering/render_entities.hpp"

class Renderer {
public:
    Renderer() {};
    ~Renderer() {};
    bool Init();

    void UpdateTick(const int tick) { this->m_tick = tick; }
    int GetTick() { return this->m_tick; }

    void UpdateResolution(const int w, const int h) { this->resolution = glm::fvec2(w, h); }
    glm::fvec2 GetResolution() { return this->resolution; }

    void Render(World* world, Shader* shader);
    void Shutdown();

private:
    int m_tick = 0;
    glm::fvec2 resolution;
};

#endif  // __RENDERER_H__
