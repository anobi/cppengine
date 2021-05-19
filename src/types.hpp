#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::fvec3 position;
    glm::fvec3 normal;
    glm::fvec3 tangent;
    glm::fvec3 bitangent;
    glm::fvec2 texCoords;
};

enum class lightTypes
{
    NONE,
    SPOTLIGHT,
    POINTLIGHT,
    DIRECTIONAL_LIGHT
};

#endif