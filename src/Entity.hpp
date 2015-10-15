#ifndef ENTITY_H
#define ENTITY_H

#include "Vector.hpp"

#include <vector>

typedef struct {
    vector3 normal;
    vector3 position;
    vector3 color;
} vertex_t;

typedef struct {
    std::vector<vertex_t> vertices;
    // + textures, shaders etc..
} renderEntity_t;

/////////////////
// E N T I T Y //
/////////////////

class Entity {

private:
    vector3 location;
    bool alive;
    renderEntity_t* renderEntity;

public:
    Entity();
    ~Entity();

    void Spawn();
	void Update();
    void Kill();
    void Remove();
    vector3 GetLocation();
    void SetLocation(float x, float y, float z);
    
};

#endif
