#include "entity.hpp"
#include "entitycomponent.hpp"

void Entity::Render(Renderer& renderer) 
{
    for (unsigned int i = 0; i < this->components.size(); i++)
    {
        this->components[i]->Render(renderer);
    }
}

void Entity::Cleanup()
{
    for (unsigned int i = 0; i < this->components.size(); i++)
    {
        this->components[i]->Cleanup();
    }
}

void Entity::Update() {}

void Entity::AddComponent(EntityComponent* component) 
{
    component->SetParent(this);
    this->components.push_back(component);
}

EntityComponent* Entity::GetComponent(const char* name) 
{
    for (unsigned int i = 0; i < this->components.size(); i++) 
    {
        if (this->components[i]->name == name)
        {
            return this->components[i];
        }
    }
}
