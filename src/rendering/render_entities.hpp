#ifndef __REN__RENDER_ENTITIES_H__
#define __REN__RENDER_ENTITIES_H__

#include "../containers/array.hpp"

constexpr auto MAX_RENDER_ENTITIES = 1024;

struct RenderEntities
{
    unsigned int num_render_entities;
    Array<unsigned int, MAX_RENDER_ENTITIES> VAOs;
    Array<unsigned int, MAX_RENDER_ENTITIES> VBOs;
    Array<unsigned int, MAX_RENDER_ENTITIES> EBOs;
    Array<unsigned int, MAX_RENDER_ENTITIES> indices;
};

#endif  // __REN__RENDER_ENTITIES_H__