#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
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
    Entity(const std::string name) {
        this->mEntityName = name;
    }
    ~Entity();

    void Update();
    void Render(Renderer &renderer);

    void SetName(const std::string name) { this->mEntityName = name; };
    const std::string GetName() const { return this->mEntityName; }

    void AddComponent(std::shared_ptr<EntityComponent> component);
    std::shared_ptr<EntityComponent> GetComponent(const std::string componentName);

    Transform& GetTransform() { return mTransform; }

private:
    std::string mEntityName;
    std::vector<std::shared_ptr<EntityComponent>> mComponents;
    Transform mTransform;
};

#endif
