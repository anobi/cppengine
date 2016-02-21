#include "Entity.hpp"
#include "EntityComponent.hpp"

Entity::~Entity() {}

void Entity::Render(Renderer& renderer){
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		this->mComponents[i]->Render(renderer);
	}
}

void Entity::Update(){}

void Entity::AddComponent(EntityComponent* component) {
	this->mComponents.push_back(component);
	component->AddToParent(this);
}

EntityComponent* Entity::GetComponent(const std::string name) {

	EntityComponent* component = NULL;

	for (unsigned int i = 0; i < mComponents.size(); i++) {
		if (mComponents[i]->GetName() == name) {
			component = mComponents[i];
			break;
		}
	}

	return component;
}
