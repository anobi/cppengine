#include "Entity.hpp"
#include "EntityComponent.hpp"

Entity::~Entity() {}

void Entity::Render(Shader& shader, const glm::mat4 viewProjection){
	for (unsigned int i = 0; i < mComponents.size(); i++) {
		this->mComponents[i]->Render(shader, viewProjection);
	}
}

void Entity::Update(){}

void Entity::AddComponent(EntityComponent* component) {
	this->mComponents.push_back(component);
	component->AddToParent(this);
}
