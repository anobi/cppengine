#ifndef ENTITY_H
#define ENTITY_H

#include "lib/Vector.hpp"
#include <vector>

typedef struct {
    vector3<float> normal;
    vector3<float> position;
    vector3<float> color;
} vertex_t;

typedef struct {
    std::vector<vertex_t> vertices;
} renderEntity_t;

/////////////////
// E N T I T Y //
/////////////////

class Entity {

private:
    vector3<float> location;
    bool alive;
    renderEntity_t* renderEntity;

public:
    Entity();
    ~Entity();

    void Spawn();
    void Kill();
    void Remove();
    vector3<float> GetLocation();
    void SetLocation(float x, float y, float z);
    
};

#endif
