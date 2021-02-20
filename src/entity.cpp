#include "entity.hpp"
#include "entitycomponent.hpp"

Entity::~Entity() {
    for (unsigned int i = 0; i < this->components.size(); i++) {
        this->components[i]->~EntityComponent();
    }
}

void Entity::Render(Renderer& renderer) {
    for (unsigned int i = 0; i < this->components.size(); i++) {
        this->components[i]->Render(renderer);
    }
}

void Entity::Update() {}

void Entity::AddComponent(EntityComponent* component) {
    component->SetParent(this);
    this->components.push_back(component);
}

EntityComponent* Entity::GetComponent(const std::string name) {
    for (unsigned int i = 0; i < this->components.size(); i++) {
        if (this->components[i]->GetName() == name) {
            return this->components[i];
        }
    }
}
