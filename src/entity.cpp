#include "entity.hpp"
#include "entitycomponent.hpp"

Entity::~Entity() {
    for (unsigned int i = 0; i < mComponents.size(); i++) {
        mComponents[i]->~EntityComponent();
    }
}

void Entity::Render(Renderer &renderer){
    for (unsigned int i = 0; i < mComponents.size(); i++) {
        mComponents[i]->Render(renderer);
    }
}

void Entity::Update(){}

void Entity::AddComponent(std::shared_ptr<EntityComponent> component) {
    component->SetParent(this);
    mComponents.push_back(component);
}

std::shared_ptr<EntityComponent> Entity::GetComponent(const std::string name) {
    for (unsigned int i = 0; i < mComponents.size(); i++) {
        if (mComponents[i]->GetName() == name) {
            return mComponents[i];
        }
    }
}
