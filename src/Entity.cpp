#include "Entity.hpp"
#include "EntityComponent.hpp"

Entity::~Entity() {}

void Entity::Render(Shader& shader, Camera& camera) {
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		this->mComponents[i]->Render(shader, camera);
	}
}

void Entity::Update(){}

void Entity::AddComponent(EntityComponent* component) {
	this->mComponents.push_back(component);
	component->AddToParent(this);
}
