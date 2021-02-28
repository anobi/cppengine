#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <vector>

#include "transform.hpp"

/////////////////
// E N T I T Y //
/////////////////

class Renderer;
class Camera;
class EntityComponent;
class Entity;

class Entity {
public:
    Entity() {}
    Entity(const char* name) { this->name = name; }
    ~Entity() {};

    void Update();
    void Render(Renderer& renderer);
    void Cleanup();

    void AddComponent(EntityComponent* component);
    EntityComponent* GetComponent(const char* componentName);

    const char* name;
    std::vector<EntityComponent*> components;
    Transform transform;
};

#endif
