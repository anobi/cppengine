#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <memory>

#include "transform.hpp"
#include "entity.hpp"

class EntityComponent {
public:
    EntityComponent() {}
    virtual ~EntityComponent() {}

    virtual void Update() {}
    virtual void Render(Renderer& renderer) {}
    virtual void AddToParent(Entity* parent) { SetParent(parent); }

    void SetParent(Entity* parent) { 
        this->parent = parent;
        this->transform = &parent->transform;
    }

    std::string name;
    Entity* parent;
    Transform* transform;
};

#endif
