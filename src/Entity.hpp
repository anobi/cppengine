#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

/////////////////
// E N T I T Y //
/////////////////

class Entity {

private:

public:

	glm::fvec3 location;
	bool alive;

    Entity();
    ~Entity();

	void Init();
    void Spawn();
	void Update();
    void Kill();
    void Remove();
};

#endif
