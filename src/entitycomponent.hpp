#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <string>
#include <memory>

#include "transform.hpp"
#include "entity.hpp"

class EntityComponent {
public:
    EntityComponent() {}
    virtual ~EntityComponent() {}

    virtual void Update() {}
    virtual void AddToParent(Entity* parent) { SetParent(parent); }
    virtual void Cleanup() {};

    void SetParent(Entity* parent) { 
        this->parent = parent;
        this->transform = &parent->transform;
    }

    std::string name;
    Entity* parent;
    Transform* transform;
};

#endif
