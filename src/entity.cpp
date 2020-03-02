#include "entity.hpp"
#include "entitycomponent.hpp"

Entity::~Entity() {
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		mComponents[i]->~EntityComponent();
	}
}

void Entity::Update(){}

void Entity::AddComponent(std::shared_ptr<EntityComponent> component) {
	component->SetParent((std::shared_ptr<Entity>) this);
	mComponents.push_back(component);
}

std::shared_ptr<EntityComponent> Entity::GetComponent(const std::string name) {
	std::shared_ptr<EntityComponent> component = nullptr;
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		if (mComponents[i]->GetName() == name) {
			component = mComponents[i];
		}
	}
	return component;
}
